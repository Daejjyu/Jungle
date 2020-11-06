print('Hello, jungle!')

print('a'.upper())

# string의 곱연산은 연결되는 결과로 나온다.
print('a' * 10)

# 리스트의 덧셈/ 곱은 연결되는 결과로 나온다.
a = [3, 3, 1]
b = [5, 2]
print(a + b)
print(a * 2)


# 함수정의
def f(x):
    return 2 * x + 3


print(f(2))


def isEven(x):
    if (x % 2 == 0):
        return True
    else:
        return False


print(isEven(2))
print(isEven(5))

#############################################
print('-' * 50)
print("과일 숫자 세기")

fruits = ['사과', '배', '배', '감', '수박', '귤', '딸기', '사과', '배', '수박']


def countName(name):
    cnt = 0
    for i in fruits:
        if (name == i):
            cnt += 1
    return cnt


print(countName('사과'))
print(countName('배'))

#############################################
print('-' * 50)
print('사람의 나이 출력하기')

people = [{'name': 'bob', 'age': 20},
          {'name': 'carry', 'age': 38},
          {'name': 'john', 'age': 7},
          {'name': 'smith', 'age': 17},
          {'name': 'ben', 'age': 27}]


def howOldAreYou(name):
    for i in people:
        if (name == i['name']):
            return i['age']
    return 'no name here'


print(howOldAreYou('bob'))
