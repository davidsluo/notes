import argparse
import heapq
import math
from typing import TextIO, List

import networkx
from matplotlib import pyplot


class Node:
    def __init__(self, label, predecessor: 'Node' = None, distance=math.inf):
        self.label = label
        self.predecessor = predecessor
        self.distance = distance

    def __str__(self):
        predecessor = self.predecessor.label if self.predecessor is not None else 'N'
        return f'({self.label}, {predecessor}, {self.distance})'

    def __repr__(self):
        predecessor = self.predecessor.label if self.predecessor is not None else 'None'
        return f'Node(label={self.label}, predecessor={predecessor}, distance={self.distance})'

    def __lt__(self, other: 'Node'):
        return self.distance < other.distance

    def __gt__(self, other: 'Node'):
        return self.distance > other.distance

    def __hash__(self):
        return hash(self.label)


def prims(g: networkx.Graph):
    start = list(g.nodes.keys())[0]
    start.distance = 0

    heap: List[Node] = []
    for node in g.nodes.keys():
        heapq.heappush(heap, node)

    while len(heap) > 0:
        current: Node = heapq.heappop(heap)
        for neighbor in g.neighbors(current):
            edge_weight = g[current][neighbor]['weight']
            if neighbor in heap and edge_weight < neighbor.distance:
                neighbor.predecessor = current
                neighbor.distance = edge_weight

    tree = [(node, node.predecessor) for node in g.nodes.keys() if node.predecessor is not None]
    return tree


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file', type=argparse.FileType(mode='r'))
    args = parser.parse_args()

    graph = networkx.Graph()

    node_count = None
    start_node = None
    end_node = None
    weight_table = {}

    file: TextIO = args.input_file
    with file as f:
        # read node info
        for _ in range(3):
            key, value = f.readline().split('=', maxsplit=1)
            if key == 'n':
                node_count = int(value)
            elif key == 's':
                start_node = int(value)
            elif key == 't':
                end_node = int(value)

        graph.add_nodes_from([Node(i) for i in range(1, node_count + 1)])

        f.readline()

        for node1 in range(1, node_count + 1):
            for node2, weight in enumerate(f.readline().split(', '), start=1):
                if node1 == node2:
                    continue
                u = next(filter(lambda x: x.label == node1, graph.nodes.keys()))
                v = next(filter(lambda x: x.label == node2, graph.nodes.keys()))
                graph.add_edge(u, v, weight=int(weight))

    tree = prims(graph)

    pyplot.figure(dpi=300)
    networkx.draw_shell(graph, with_labels=True,
                        node_color='red', font_color='black', font_weight='bold', font_family='Iosevka')
    networkx.draw_networkx_edges(graph, networkx.shell_layout(graph), edgelist=tree, edge_color='green')
    networkx.draw_networkx_edge_labels(graph, networkx.shell_layout(graph), font_family='Iosevka', font_weight='bold')
    pyplot.show()

    total_weight = sum(attrs['weight'] for edge, attrs in graph.edges.items() if edge in tree)
    print(total_weight)
