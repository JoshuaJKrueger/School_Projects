'''
    A linked list for course objects
'''

class CourseList:
    def __init__(self):
        self.head = None


    def insert(self, Course):
        '''
            Add Course to the list in order of its Course Number
        '''

        if self.head is None:
            self.head = Course

            return

        if Course.number() < self.head.number():
            Course.next = self.head
            self.head = Course

            return

        prev_course = None
        for course in self:
            if prev_course:
                if Course.number() > prev_course.number() and Course.number() < course.number():
                    prev_course.next = Course
                    Course.next = course

                    break
            if course.next:
                prev_course = course
            else:
                course.next = Course


    def remove(self, number):
        '''
            Remove the first Course with Course Number of number
        '''

        prev_course = None
        for course in self:
            if course.number() == number:
                if prev_course:
                    prev_course.next = course.next

                    break
            else:
                prev_course = course


    def remove_all(self, number):
        '''
            Removes all Courses with Course Number of number
        '''

        prev_course = None
        for course in self:
            if course.number() == number:
                if prev_course:
                    prev_course.next = course.next
            else:
                prev_course = course


    def find(self, number):
        '''
            Return the first Course with Course Number of number or return -1
        '''

        for course in self:
            if course.number() == number:
                return course

        return -1


    def size(self):
        '''
            Return the number of items in the list
        '''

        count = 0
        for _ in self:
            count += 1

        return count


    def calculate_gpa(self):
        '''
            Return the gpa from all course in list
        '''

        if self.size() < 1:
            return 0.0

        credits = 0
        grades = 0
        for course in self:
            credits += course.credit_hr()
            grades += course.grade() * course.credit_hr()

        return grades / credits

    def is_sorted(self):
        '''
            Return True if sorted by Course Number, false otherwise
        '''

        prev_course_number = None
        for course in self:
            if prev_course_number:
                if prev_course_number > course.number():
                    return False
            else:
                prev_course_number = course.number()

        return True


    def __str__(self):
        '''
            Return all stored courses info
        '''

        return_string = ''

        for course in self:
            return_string += str(course)

        return return_string


    def __iter__(self):
        self.current_node = None
        self.next_node = self.head

        return self


    def __next__(self):
        if self.next_node is None:
            raise StopIteration

        self.current_node = self.next_node
        self.next_node = self.next_node.next

        return self.current_node
