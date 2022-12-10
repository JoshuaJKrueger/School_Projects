'''
    This file contains a Binary Search Tree implementation
'''


class Node:
    '''
        This class is a node struct for BST
    '''
    def __init__(self, data, is_left=True, parent=None):
        self.data = data
        self.is_left = is_left
        self.parent = parent
        self.left = None
        self.right = None


class BST:
    '''
        This class is a Binary Search Tree ADT
    '''
    def __init__(self):
        self.root = None


    def is_empty(self):
        '''
            Return True if empty, False otherwise
        '''

        return not bool(self.root)


    def size(self, node=None, is_start=True):
        '''
            Return the number of items in the tree
        '''

        if is_start:
            node = self.root
            is_start = False

        return self.size(node.left, is_start) + self.size(node.right, is_start) + 1 if node else 0


    def height(self, node=None, is_start=True):
        '''
            Return the height of the tree
        '''
        if is_start:
            node = self.root
            is_start = False

        return (max(self.height(node.left, is_start), self.height(node.right, is_start)) + 1
                if node else 0)


    def add(self, new_data, node=None, is_start=True):
        '''
            Add item to its proper place in the tree. Return the modified tree
        '''

        if is_start:
            new_data = Node(new_data)
            node = self.root
            is_start = False

        if node:
            if new_data.data == node.data:
                node.data.count += 1
            elif new_data.data < node.data:
                if node.left:
                    self.add(new_data, node.left, is_start)
                else:
                    new_data.parent = node
                    node.left = new_data
            elif node.right:
                self.add(new_data, node.right, is_start)
            else:
                new_data.parent = node
                new_data.is_left = False
                node.right = new_data
        else:
            new_data.is_left = False
            self.root = new_data

        return self


    def remove(self, target, node=None, is_start=True):
        '''
            Remove item from the tree. Return the modified tree
        '''

        if is_start:
            node = self.root
            is_start = False

        if target == node.data:
            if node.left and node.right:
                temp = self.get_left_most_node(node.right)
                self.remove(temp.data)
                temp.left = node.left
                temp.right = node.right
            elif node.left:
                temp = node.left
            elif node.right:
                temp = node.right
            else:
                temp = None

            if node.parent:
                if node.is_left:
                    node.parent.left = temp
                else:
                    node.parent.right = temp
            else:
                temp.parent = None
                self.root = temp
        elif target < node.data and node.left:
            self.remove(target, node.left, is_start)
        elif node.right:
            self.remove(target, node.right, is_start)

        return self


    def find(self, target, node=None, is_start=True):
        '''
            Return the matched item. If item is not in the tree, raise a ValueError
        '''

        if is_start:
            target = Node(target)
            node = self.root
            is_start = False

        if node:
            if target.data == node.data:
                return node
            elif target.data < node.data and node.left:
                self.find(target, node.left, is_start)
            elif node.right:
                self.find(target, node.right, is_start)

        raise ValueError


    def in_order(self, node=None, is_start=True):
        '''
            Return a list with the data items in order of in_order traversal
        '''

        if is_start:
            node = self.root
            is_start = False

        return (self.in_order(node.left, is_start) + [node.data] + self.in_order(node.right, is_start)
                if node else [])


    def pre_order(self, node=None, is_start=True):
        '''
            Return a list with the data items in order of pre_order traversal
        '''

        if is_start:
            node = self.root
            is_start = False

        return ([node.data] + self.pre_order(node.left, is_start) +
                self.pre_order(node.right, is_start) if node else [])


    def post_order(self, node=None, is_start=True):
        '''
            Return a list with the data items in order of post_order traversal
        '''

        if is_start:
            node = self.root
            is_start = False

        return (self.post_order(node.left, is_start) + self.post_order(node.right, is_start) +
                [node.data] if node else [])


    def rebalance(self, nodes=None, is_start=True):
        '''
            Rebalance the tree. Return the modified tree
        '''

        if is_start:
            nodes = self.in_order()
            self.root = None
            is_start = False

        mid = len(nodes) // 2
        left, root, right = (nodes[:mid], nodes[mid], nodes[mid:])

        self.add(root)

        if len(left) == 0 and len(right) < 2:
            return None
        if len(left) != 0:
            self.rebalance(left, is_start)
        if len(right) > 1:
            self.rebalance(right, is_start)

        return self


    def get_left_most_node(self, node):
        '''
            Returns the left most node from the input node
        '''
        return self.get_left_most_node(node.left) if node.left else node
