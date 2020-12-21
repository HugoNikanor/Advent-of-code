
## sample
# last_spoken = {
#         0: [1],
#         3: [2],
#         6: [3],
#         }
# last_number = 6

last_spoken = {
        6 : [1],
        13: [2],
        1 : [3],
        15: [4],
        2 : [5],
        0 : [6],
         }
last_number = 0

output = []

turn = len(last_spoken)
# for number in input:
while True:
    # if turn == 2020: break
    if turn == 30_000_000: 
        print(last_spoken)
        break
    turn += 1
    # print(f'{turn:2} ', end='')
    if m := last_spoken.get(last_number):
        if len(m) == 1:
            # print(0)
            last_spoken[0].append(turn)
            last_number = 0
        else:    
            n = m[-1] - m[-2]
            # print(n)
            last_number = n
            if v := last_spoken.get(n):
                v.append(turn)
            else:
                last_spoken[n] = [turn]

    else: # haven't been said before
        # print(0)
        last_spoken[0] = [turn]
        last_number = 0

