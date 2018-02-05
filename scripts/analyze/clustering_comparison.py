#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

import json
import glob
import sys
import re
import multiprocessing

from os import listdir, path
from subprocess import check_output

from networkit import *

data = {}

for arg in sys.argv[1:]:
  for file in glob.glob(arg):
    for typename, items in json.load(open(file)).items():
      if typename in data:
        data[typename].update(items)
      else:
        data[typename] = items

frames = { typename: pd.DataFrame.from_dict(items, orient='index') for typename, items in data.items() }

def read_clustering(clustering_file):
  if clustering_file.endswith(".txt"):
    return community.readCommunities(clustering_file)
  else:
    files = sorted(glob.glob(clustering_file))
    return community.BinaryEdgeListPartitionReader(0, 4).read(files)

def work(rows):
  row1, row2 = rows
  index1, row_data1 = row1
  index2, row_data2 = row2

  if index1 == index2:
    return
  if 'clustering_comparison' in frames:
    if frames['clustering_comparison'].loc[lambda x: x.base_clustering_id == index1].loc[lambda x: x.compare_clustering_id == index2].any():
      return
    if frames['clustering_comparison'].loc[lambda x: x.base_clustering_id == index2].loc[lambda x: x.compare_clustering_id == index1].any():
      return

  clustering1 = read_clustering(row_data1['path'])
  clustering2 = read_clustering(row_data2['path'])

  g = graph.Graph(clustering1.numberOfElements())
  nmi = 1.0 - community.NMIDistance().getDissimilarity(g, clustering1, clustering2)
  ari = 1.0 - community.AdjustedRandMeasure().getDissimilarity(g, clustering1, clustering2)

  tmpfile_name = "_tmp_{}_{}".format(index1, index2)
  tmpfile = open(tmpfile_name, "w")
  tmpfile.write("#LOG# clustering_comparison/0/base_clustering_id: {}\n".format(index1))
  tmpfile.write("#LOG# clustering_comparison/0/compare_clustering_id: {}\n".format(index2))
  tmpfile.write("#LOG# clustering_comparison/0/NMI: {}\n".format(nmi))
  tmpfile.write("#LOG# clustering_comparison/0/ARI: {}\n".format(ari))
  tmpfile.close()
  check_output(["./report_to_json.rb", tmpfile_name])
  check_output(["rm", tmpfile_name])

count = multiprocessing.cpu_count()
pool = multiprocessing.Pool(processes=count)

graphs_with_clusterings = frames['clustering'] \
  .merge(frames['algorithm_run'], left_on='algorithm_run_id', right_index=True) \
  .merge(frames['program_run'], left_on='program_run_id', right_index=True) \
  .groupby('graph')

for graph, clusterings in graphs_with_clusterings:
  pool.map(work, ((row1, row2) for row1 in clusterings.iterrows() for row2 in clusterings.iterrows()))

  ground_truth = frames['clustering'].loc[lambda x: x.source == "ground_truth"].loc[lambda x: x.graph == graph]
  pool.map(work, ((row1, row2) for row1 in ground_truths.iterrows() for row2 in clusterings.iterrows()))
