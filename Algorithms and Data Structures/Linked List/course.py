'''
    Course Object to store all the info about a given course.
    Used as data in CourseList(linked list)
'''

class Course:
    def __init__(self, num=0, title='', course_hr=0.0, grade_point=0.0):
        if not isinstance(num, int) or num < 0:
            raise ValueError
        if not isinstance(title, str):
            raise ValueError
        if not isinstance(course_hr, float) or course_hr < 0:
            raise ValueError
        if not isinstance(grade_point, float) or grade_point < 0:
            raise ValueError

        self.num = num
        self.title = title
        self.course_hr = course_hr
        self.grade_point = grade_point
        self.next = None


    def number(self):
        '''
            Return Course Number as int
        '''

        return int(self.num)


    def name(self):
        '''
            Return Course Name as string
        '''

        return str(self.title)


    def credit_hr(self):
        '''
            Return Credits as float
        '''

        return float(self.course_hr)


    def grade(self):
        '''
            Return GP
        '''

        return float(self.grade_point)


    def __str__(self):
        '''
            Return Course info; exempli gratia, "cs1030 Introduction to
             Computers Grade: 3.2 Credit Hours: 2.0"
        '''

        return f'CS-{self.num}: {self.title} Grade: {self.grade_point} Credit Hours: {self.course_hr}\n'
