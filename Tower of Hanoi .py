from math import pow
def HanoiTower(n):
    moves = int(pow(2,n)-1)
    a = [n+1] + list(range(n,0,-1))
    b = [n+1]
    c = [n+1]
    for i in range(1,moves+1):
        if i%3 == 1:
            if a[-1] > c[-1]:
                a.append(c.pop())
                print('C --> A')
            else:
                c.append(a.pop())
                print('A --> C')
        elif i%3 == 2:
            if a[-1] > b[-1]:
                a.append(b.pop())
                print('B --> A')
            else:
                b.append(a.pop())
                print('A --> B')
        else:
            if b[-1] > c[-1]:
                b.append(c.pop())
                print('C --> B')
            else:
                c.append(b.pop())
                print('B --> C')
HanoiTower(4)

