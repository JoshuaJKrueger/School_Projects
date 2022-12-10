'''
    Contains Custom Stack ADT and its necessary class
'''

class Element:
    '''
        An element to be contained by Stack
    '''
    def __init__(self, value):
        self.value = value
        self.next = None


class Stack:
    '''
        Custom Stack ADT using linked lists
    '''
    def __init__(self):
        self.anchor = Element(None)
        self.num_of_elements = 0


    def push(self, item:str):
        '''
            Pushes item onto stack (becomes top element)
        '''
        elem = Element(item)

        elem.next = self.anchor.next
        self.anchor.next = elem
        self.num_of_elements += 1



    def pop(self):
        '''
            Removes the top element and returns it
        '''
        if self.num_of_elements == 0:
            raise IndexError

        return_elem = self.anchor.next
        self.anchor.next = return_elem.next
        self.num_of_elements -= 1

        return return_elem.value


    def top(self):
        '''
            Return top element (without removing it)
        '''
        if self.num_of_elements == 0:
            raise IndexError

        return self.anchor.next.value


    def size(self):
        '''
            Return the number of items in the stack
        '''
        return self.num_of_elements


    def clear(self):
        '''
            Empty the stack
        '''
        self.anchor.next = None
