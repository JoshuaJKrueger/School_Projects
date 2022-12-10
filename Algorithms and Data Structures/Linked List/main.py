'''
    Joshua Krueger
    CS 2420-002 Project 3
    This program create a linked list for school courses
'''

import course
import courselist

def main():
    '''
        Entry point of program
    '''
    course_list = courselist.CourseList()

    for line in open('data.txt', 'r').readlines():
        num, title, hours, grade = line.strip('\n').split(',')
        num = int(num)
        title = str(title)
        hours = float(hours)
        grade = float(grade)
        course_list.insert(course.Course(num, title, hours, grade))

    print(f'List size: {course_list.size()}')
    print(course_list)
    print(f'Cumulative GPA: {course_list.calculate_gpa()}')

if __name__ == '__main__':
    main()
