"""
    将属性命名以单下划线开头，通过这种方式来暗示属性是受保护的，不建议外界直接访问，那么如果想访问属性可以
通过属性的 getter(访问器)和 setter(修改器)方法进行对应的操作。
"""

class Person(object):

    def __init__(self, name, age):
        self._name = name
        self._age = age


    # 访问器 - getter方法
    @property
    def name(self):
        return self._name

    # 访问器 - getter 方法
    @property
    def age(self, age):
        return self._age

    # 修改器 -  setter 方法
    @age.setter
    def age(self, age):
        self._age = age

    def play(self):
        if self._age <= 16:
            print("%s正在玩儿飞行棋."%self._name)
        else:
            print("%s正在玩儿斗地主." % self._name)

def main():
    person = Person("王大厨", 12)
    person.play()
    person.age = 22
    person.play()

if __name__ == '__main__':
    main()