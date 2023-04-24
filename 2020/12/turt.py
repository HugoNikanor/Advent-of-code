#!/usr/bin/env python

import turtle

def dir_vec(action, magnitude):

    return {
            'N': (0, magnitude),
            'W': (-magnitude, 0),
            'S': (0, -magnitude),
            'E': (magnitude, 0),
            }[action]

try:
    # begin_fill()
    while True:
        line = input()
        action = line[0]
        num = int(line[1:])

        proc = {
                'F': turtle.forward,
                'B': turtle.backward,
                'L': turtle.left,
                'R': turtle.right,
        }.get(action, lambda m: turtle.setpos([a + b for a, b 
                    in zip(turtle.position(), dir_vec(action, m))]))

        proc(num)
        print(f'{action}\t{num}\t{turtle.position()}')




except EOFError:
    turtle.done()
    pass


"""
Canvas
Pen
RawPen
RawTurtle
Screen
ScrolledCanvas
Shape
TK
TNavigator
TPen
Tbuffer
Terminator
Turtle
TurtleGraphicsError
TurtleScreen
TurtleScreenBase
Vec2D
_CFG
_LANGUAGE
_Root
_Screen
_TurtleImage
__all__
__builtins__
__cached__
__doc__
__file__
__forwardmethods
__func_body
__loader__
__methodDict
__methods
__name__
__package__
__spec__
__stringBody
_alias_list
_make_global_funcs
_screen_docrevise
_tg_classes
_tg_screen_functions
_tg_turtle_functions
_tg_utilities
_turtle_docrevise
_ver
addshape
back
backward
begin_fill
begin_poly
bgcolor
bgpic
bk
bye
circle
clear
clearscreen
clearstamp
clearstamps
clone
color
colormode
config_dict
deepcopy
degrees
delay
distance
done
dot
down
end_fill
end_poly
exitonclick
fd
fillcolor
filling
forward
get_poly
get_shapepoly
getcanvas
getmethparlist
getpen
getscreen
getshapes
getturtle
goto
heading
hideturtle
home
ht
inspect
isdown
isfile
isvisible
join
left
listen
lt
mainloop
math
mode
numinput
onclick
ondrag
onkey
onkeypress
onkeyrelease
onrelease
onscreenclick
ontimer
pd
pen
pencolor
pendown
pensize
penup
pos
position
pu
radians
read_docstrings
readconfig
register_shape
reset
resetscreen
resizemode
right
rt
screensize
seth
setheading
setpos
setposition
settiltangle
setundobuffer
setup
setworldcoordinates
setx
sety
shape
shapesize
shapetransform
shearfactor
showturtle
simpledialog
speed
split
st
stamp
sys
textinput
tilt
tiltangle
time
title
towards
tracer
turtles
turtlesize
types
undo
undobufferentries
up
update
width
window_height
window_width
write
write_docstringdict
xcor
ycor
"""
