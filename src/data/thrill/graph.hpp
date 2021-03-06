#pragma once

#include <thrill/api/dia.hpp>
#include <thrill/api/group_to_index.hpp>

#include <cstdint>
#include <iostream>
#include <algorithm>

using NodeId = uint32_t;
using Weight = uint64_t;
using ClusterId = uint32_t;

using NodeCluster = std::pair<NodeId, ClusterId>;

struct Edge;
struct WeightedEdge;
struct NodeWithLinks;
struct NodeWithWeightedLinks;
struct EdgeTargetWithDegree;
struct WeightedEdgeTargetWithDegree;

struct EdgeTarget {
  using EdgeType = Edge;

  NodeId target;

  inline NodeId getTarget() const { return target; }
  inline Weight getWeight() const { return 1; }

  static EdgeTarget fromEdge(const Edge& edge);
  static EdgeTarget fromLinkWithTargetDegree(const EdgeTargetWithDegree& link);
};

struct WeightedEdgeTarget {
  using EdgeType = WeightedEdge;

  NodeId target;
  Weight weight;

  inline NodeId getTarget() const { return target; }
  inline Weight getWeight() const { return weight; }

  static WeightedEdgeTarget fromEdge(const WeightedEdge& edge);
  static WeightedEdgeTarget fromLinkWithTargetDegree(const WeightedEdgeTargetWithDegree& link);
};

struct EdgeTargetWithDegree {
  using EdgeType = Edge;

  NodeId target;
  uint32_t target_degree; // this assuemes that degrees in first level will fit into 32bit integer

  inline NodeId getTarget() const { return target; }
  inline Weight getWeight() const { return 1; }

  static EdgeTargetWithDegree fromLink(const EdgeTarget& link, Weight target_degree) {
    return EdgeTargetWithDegree { link.target, uint32_t(target_degree) };
  }
};
EdgeTarget EdgeTarget::fromLinkWithTargetDegree(const EdgeTargetWithDegree& link) {
  return EdgeTarget { link.target };
}

struct WeightedEdgeTargetWithDegree {
  using EdgeType = WeightedEdge;

  NodeId target;
  Weight weight;
  Weight target_degree;

  inline NodeId getTarget() const { return target; }
  inline Weight getWeight() const { return weight; }

  static WeightedEdgeTargetWithDegree fromLink(const WeightedEdgeTarget& link, Weight target_degree) {
    return WeightedEdgeTargetWithDegree { link.target, link.weight, target_degree };
  }
};
WeightedEdgeTarget WeightedEdgeTarget::fromLinkWithTargetDegree(const WeightedEdgeTargetWithDegree& link) {
  return WeightedEdgeTarget { link.target, link.weight };
}

struct Edge {
  using NodeType = NodeWithLinks;

  NodeId tail, head;

  Weight getWeight() const {
    return 1;
  }

  void flip() {
    NodeId tmp = tail;
    tail = head;
    head = tmp;
  }

  static Edge fromLink(NodeId node, const EdgeTarget& link) {
    return Edge { node, link.target };
  }
};
std::ostream& operator << (std::ostream& os, Edge& e) {
  return os << e.tail << " -> " << e.head;
}
EdgeTarget EdgeTarget::fromEdge(const Edge& edge) {
  return EdgeTarget { edge.head };
}

struct WeightedEdge {
  using NodeType = NodeWithWeightedLinks;

  NodeId tail, head;
  Weight weight;

  Weight getWeight() const {
    return weight;
  }

  void flip() {
    NodeId tmp = tail;
    tail = head;
    head = tmp;
  }

  static WeightedEdge fromLink(NodeId node, const WeightedEdgeTarget& link) {
    return WeightedEdge { node, link.target, link.weight };
  }
};
std::ostream& operator << (std::ostream& os, WeightedEdge& e) {
  return os << e.tail << " -> " << e.head << " (" << e.weight << ")";
}
WeightedEdgeTarget WeightedEdgeTarget::fromEdge(const WeightedEdge& edge) {
  return WeightedEdgeTarget { edge.head, edge.weight };
}

struct NodeWithWeightedLinks {
  using LinkType = WeightedEdgeTarget;

  NodeId id;
  std::vector<WeightedEdgeTarget> links;
  Weight weighted_degree_cache = 0;

  Weight weightedDegree() const { return weighted_degree_cache; }

  Weight loopWeight() const {
    Weight loops = 0;
    for (const WeightedEdgeTarget& link : links) {
      if (link.target == id) {
        loops += link.weight;
      }
    }
    return loops;
  }

  void push_back(const WeightedEdgeTarget& link) {
    weighted_degree_cache += link.weight;
    links.push_back(link);
  }

  NodeWithWeightedLinks asNodeWithWeights() const {
    return *this;
  }
};

struct NodeWithLinks {
  using LinkType = EdgeTarget;

  NodeId id;
  std::vector<EdgeTarget> links;

  Weight weightedDegree() const { return links.size(); }

  Weight loopWeight() const {
    Weight loops = 0;
    for (const EdgeTarget& link : links) {
      if (link.target == id) {
        loops++;
      }
    }
    return loops;
  }

  void push_back(const EdgeTarget& link) {
    links.push_back(link);
  }

  NodeWithWeightedLinks asNodeWithWeights() const {
    NodeWithWeightedLinks node { id, {} };
    node.links.reserve(links.size());
    for (const EdgeTarget& link : links) {
      node.push_back(WeightedEdgeTarget { link.target, 1 });
    }
    return node;
  }
};

namespace thrill {
namespace data {
template <typename Archive>
struct Serialization<Archive, NodeWithLinks>{
  static void Serialize(const NodeWithLinks& node, Archive& ar) {
    Serialization<Archive, NodeId>::Serialize(node.id, ar);
    Serialization<Archive, std::vector<EdgeTarget>>::Serialize(node.links, ar);
  }
  static NodeWithLinks Deserialize(Archive& ar) {
    return NodeWithLinks { Serialization<Archive, NodeId>::Deserialize(ar), Serialization<Archive, std::vector<EdgeTarget>>::Deserialize(ar) };
  }
  static constexpr bool is_fixed_size = false;
  static constexpr size_t fixed_size = 0;
};

template <typename Archive>
struct Serialization<Archive, NodeWithWeightedLinks>{
  static void Serialize(const NodeWithWeightedLinks& node, Archive& ar) {
    Serialization<Archive, NodeId>::Serialize(node.id, ar);
    Serialization<Archive, std::vector<WeightedEdgeTarget>>::Serialize(node.links, ar);
    Serialization<Archive, Weight>::Serialize(node.weighted_degree_cache, ar);
  }
  static NodeWithWeightedLinks Deserialize(Archive& ar) {
    return NodeWithWeightedLinks { Serialization<Archive, NodeId>::Deserialize(ar), Serialization<Archive, std::vector<WeightedEdgeTarget>>::Deserialize(ar), Serialization<Archive, Weight>::Deserialize(ar) };
  }
  static constexpr bool is_fixed_size = false;
  static constexpr size_t fixed_size = 0;
};
} // data
} // thrill


struct NodeWithLinksAndTargetDegree {
  using LinkType = EdgeTargetWithDegree;

  NodeId id;
  std::vector<EdgeTargetWithDegree> links;

  Weight weightedDegree() const { return links.size(); }

  void push_back(const EdgeTargetWithDegree& link) {
    links.push_back(link);
  }

  NodeWithLinks toNodeWithoutTargetDegrees() const {
    NodeWithLinks node { id, {} };
    node.links.reserve(links.size());
    for (const auto& link : links) {
      node.push_back(EdgeTarget::fromLinkWithTargetDegree(link));
    }
    return node;
  }
};


struct NodeWithWeightedLinksAndTargetDegree {
  using LinkType = WeightedEdgeTargetWithDegree;

  NodeId id;
  std::vector<WeightedEdgeTargetWithDegree> links;
  Weight weighted_degree_cache = 0;

  Weight weightedDegree() const { return weighted_degree_cache; }

  void push_back(const WeightedEdgeTargetWithDegree& link) {
    weighted_degree_cache += link.weight;
    links.push_back(link);
  }

  NodeWithWeightedLinks toNodeWithoutTargetDegrees() const {
    NodeWithWeightedLinks node { id, {} };
    node.links.reserve(links.size());
    for (const auto& link : links) {
      node.push_back(WeightedEdgeTarget::fromLinkWithTargetDegree(link));
    }
    return node;
  }
};

namespace thrill {
namespace data {
template <typename Archive>
struct Serialization<Archive, NodeWithLinksAndTargetDegree>{
  static void Serialize(const NodeWithLinksAndTargetDegree& node, Archive& ar) {
    Serialization<Archive, NodeId>::Serialize(node.id, ar);
    Serialization<Archive, std::vector<EdgeTargetWithDegree>>::Serialize(node.links, ar);
  }
  static NodeWithLinksAndTargetDegree Deserialize(Archive& ar) {
    return NodeWithLinksAndTargetDegree { Serialization<Archive, NodeId>::Deserialize(ar), Serialization<Archive, std::vector<EdgeTargetWithDegree>>::Deserialize(ar) };
  }
  static constexpr bool is_fixed_size = false;
  static constexpr size_t fixed_size = 0;
};

template <typename Archive>
struct Serialization<Archive, NodeWithWeightedLinksAndTargetDegree>{
  static void Serialize(const NodeWithWeightedLinksAndTargetDegree& node, Archive& ar) {
    Serialization<Archive, NodeId>::Serialize(node.id, ar);
    Serialization<Archive, std::vector<WeightedEdgeTargetWithDegree>>::Serialize(node.links, ar);
    Serialization<Archive, Weight>::Serialize(node.weighted_degree_cache, ar);
  }
  static NodeWithWeightedLinksAndTargetDegree Deserialize(Archive& ar) {
    return NodeWithWeightedLinksAndTargetDegree { Serialization<Archive, NodeId>::Deserialize(ar), Serialization<Archive, std::vector<WeightedEdgeTargetWithDegree>>::Deserialize(ar), Serialization<Archive, Weight>::Deserialize(ar) };
  }
  static constexpr bool is_fixed_size = false;
  static constexpr size_t fixed_size = 0;
};
} // data
} // thrill

template<typename EdgeType>
struct DiaEdgeGraph {
  using Edge = EdgeType;

  thrill::DIA<EdgeType> edges;
  size_t node_count, total_weight;
};

template<typename NodeType>
struct DiaNodeGraph {
  using Node = NodeType;

  thrill::DIA<NodeType> nodes;
  size_t node_count, total_weight;
};

template<typename NodeType, typename EdgeType>
struct DiaGraph {
  using Node = NodeType;
  using Edge = EdgeType;

  thrill::DIA<NodeType> nodes;
  thrill::DIA<EdgeType> edges;
  size_t node_count, total_weight;
};

template<typename EdgeDIA>
auto edgesToNodes(const EdgeDIA& edges, uint32_t node_count) {
  using EdgeType = typename EdgeDIA::ValueType;
  using NodeType = typename EdgeType::NodeType;
  using LinkType = typename NodeType::LinkType;

  return edges
    .template GroupToIndex<NodeType>(
      [](const EdgeType& edge) -> size_t { return edge.tail; },
      [](auto& iterator, const NodeId node) {
        NodeType first { node, {} };
        while (iterator.HasNext()) {
          first.push_back(LinkType::fromEdge(iterator.Next()));
        }
        return first;
      },
      node_count);
}

template<typename NodeDIA>
auto nodesToEdges(const NodeDIA& nodes) {
  using NodeType = typename NodeDIA::ValueType;
  using LinkType = typename NodeType::LinkType;
  using EdgeType = typename LinkType::EdgeType;

  return nodes
    .template FlatMap<EdgeType>(
      [](const NodeType& node, auto emit) {
        for (const LinkType& link : node.links) {
          emit(EdgeType::fromLink(node.id, link));
        }
      });
}
