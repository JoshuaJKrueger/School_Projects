'''
    Joshua J. Krueger
    CS 2420-002
    Project 7: Weighted Graph ADT

    This program has the classes for a weighted graph ADT.
    The Graph class has multiple searching algorithms
     already implemented. This program also contains an
     entrypoint (main) for testing.
'''

from __future__ import annotations
from typing import Generator, KeysView
from math import inf as infinity
import heapq

class Vertex:
    def __init__(self, label:str) -> None:
        """
            Initialization of class
            Vertex is used by graph as the nodes
        """

        self.label = label
        self.neighbors = {}
        self.reset()


    def reset(self):
        """
            To perform multiple searches on a graph
        """
        self.distance = infinity
        self.visited = False
        self.previous = None


    def add_neighbor(self, neighbor:Vertex, weight:float=None) -> None:
        """
            Connects self to neighbor
        """

        self.neighbors[neighbor] = weight


    def get_neighbors(self) -> KeysView:
        """
            Returns neighbors labels
        """

        return self.neighbors.keys()


    def get_weight(self, neighbor:Vertex) -> float:
        """
            Returns the weight to neighbor, infinity if not connected
        """

        for i in self.neighbors:
            if neighbor == i.label:
                return self.neighbors[i]

        return infinity


    def __lt__(self, other):
        return self.distance < other.distance


    def __le__(self, other):
        return self.distance < other.distance


    def __gt__(self, other):
        return self.distance < other.distance


    def __ge__(self, other):
        return self.distance < other.distance


class Graph:
    def __init__(self) -> None:
        """
            Weighted graph ADT initialization
        """

        self.vertices = {}


    def add_vertex(self, label:str) -> Graph:
        """
            Adds a vertex with the label
            Returns the Graph
        """

        if type(label) is not str:
            raise ValueError

        self.vertices[label] = Vertex(label)

        return self


    def add_edge(self, src:Vertex, dest:Vertex, w:float) -> Graph:
        """
            Sets the weight from src to dest
            Returns the Graph
        """

        if (src not in self.vertices or
            dest not in self.vertices or
            not (type(w) is float or
            type(w) is int)):
            raise ValueError

        self.vertices[src].add_neighbor(self.vertices[dest], w)

        return self


    def get_weight(self, src:Vertex, dest:Vertex) -> float:
        """
            Returns the weight from src to dest
        """

        if src not in self.vertices or dest not in self.vertices:
            raise ValueError

        return self.vertices[src].get_weight(dest)


    def dfs(self, starting_vertex:Vertex, label:Vertex=None, visited:list=None) -> Generator:
        """
            Returns a generator for transversing the graph in
             depth-first order starting from the passed in vertex
        """

        if starting_vertex not in self.vertices:
            raise ValueError

        visited = visited or []
        label = label or starting_vertex
        visited.append(label)
        yield label

        for neighbor in self.vertices[label].get_neighbors():
            if neighbor.label not in visited:
                yield from self.dfs(starting_vertex, neighbor.label, visited)


    def bfs(self, starting_vertex:Vertex) -> Generator:
        """
            Returns a generator for transversing the graph in
             breadth-first order starting from the passed in vertex
        """

        if starting_vertex not in self.vertices:
            raise ValueError

        visited = [starting_vertex]
        queue = [starting_vertex]

        while queue:
            current_vertex = queue.pop(0)

            yield current_vertex

            for neighbor in self.vertices[current_vertex].get_neighbors():
                if neighbor.label not in visited:
                    queue.append(neighbor.label)
                    visited.append(neighbor.label)


    def dsp(self, src:Vertex, dest:Vertex) -> tuple:
        """
            Returns a tuple of path length and a list of vertices
             transversed. If the path doesn't exist, returns (infinity, [])
        """

        if src not in self.vertices or dest not in self.vertices:
            raise ValueError

        self.vertices[src].distance = 0
        queue = [(vertex.distance, vertex) for vertex in self.vertices.values()]
        heapq.heapify(queue)

        while queue:
            nearest_vertex = heapq.heappop(queue)
            nearest_vertex[1].visited = True

            for vertex in nearest_vertex[1].get_neighbors():
                if vertex.visited:
                    continue

                new_distance = nearest_vertex[1].distance + nearest_vertex[1].get_weight(vertex.label)

                if new_distance < vertex.distance:
                    vertex.distance = new_distance
                    vertex.previous = nearest_vertex[1]

            while queue:
                heapq.heappop(queue)

            queue = [(vertex.distance, vertex) for vertex in self.vertices.values() if not vertex.visited]
            heapq.heapify(queue)

        vertex = self.vertices[dest]
        out = (vertex.distance, [dest])

        while vertex:
            if vertex.previous:
                out[1].append(vertex.previous.label)

            vertex = vertex.previous

        out[1].reverse()

        if len(out[1]) == 1:
            out[1].clear()

        for vertex in self.vertices.values():
            vertex.reset()

        return out if out[0] > 0 else (infinity, [])


    def dsp_all(self, src:Vertex) -> dict:
        """
            Uses Dijkstra's Shortest Path algorithm and returns
             a dictionary where is key is the destination and
             the value is a list of the vertices in the path
             src and dest are inclusive
        """

        if src not in self.vertices:
            raise ValueError

        out = {}

        for dest in self.vertices:
            out[dest] = self.dsp(src, dest)[1] if dest != src else [src]

        return out


    def __str__(self) -> str:
        """
            Returns a GrphViz dot notation representation of the graph
        """

        out = 'digraph G {\n'

        for key in self.vertices:
            for child_key in self.vertices[key].get_neighbors():
                w = self.get_weight(key, child_key.label)
                out += f'   {key} -> {child_key.label} [label="{w}",weight="{w}"];\n'

        return out + '}\n'


def main():
    """
        An entrypoint to test the Graph ADT
    """

    test_graph = Graph()
    [test_graph.add_vertex(label) for label in ['A', 'B', 'C', 'D', 'E', 'F']]

    test_graph.add_edge('A', 'B', 2)
    test_graph.add_edge('A', 'F', 9)
    test_graph.add_edge('B', 'C', 8)
    test_graph.add_edge('B', 'D', 15)
    test_graph.add_edge('B', 'F', 6)
    test_graph.add_edge('C', 'D', 1)
    test_graph.add_edge('E', 'C', 7)
    test_graph.add_edge('E', 'D', 3)
    test_graph.add_edge('F', 'B', 6)
    test_graph.add_edge('F', 'E', 3)

    print(test_graph)
    
    print([vertex for vertex in test_graph.dfs('A')], '\n', end='')
    print([vertex for vertex in test_graph.bfs('A')], '\n', end='')
    print(test_graph.dsp('A', 'F'))
    print(test_graph.dsp_all('A'))

main()
