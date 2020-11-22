# Basic game for static balance

import pygame, sys
import serial # import pyserial
import io # import io for pyserial

# declaring serial communication
ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM3'

# Initialize Pygame
pygame.init()

# Creating the window
WINDOW_HEIGHT = 600
WINDOW_WIDTH = 600
screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

# Setting the caption and icon
pygame.display.set_caption('Static Balance Game')

# Line
line_width = 2

# Rectangle
width = 15
height = 15
x = WINDOW_WIDTH / 2 - (width/2) + line_width
y = WINDOW_HEIGHT / 2 -(height/2) + line_width
vel = 5

# Colors
BACKGROUND_COLOR = (150, 100, 150)
BLACK = (0, 0, 0)
SHADOW = (192, 192, 192)
LIGHTBLUE= (0, 0, 255)

# Keystroke Variables
#TODO: need to be changed for balance board input
left = pygame.K_LEFT
right = pygame.K_RIGHT
up = pygame.K_UP
down = pygame.K_DOWN


def drawScreen():
    screen.fill(BACKGROUND_COLOR)

    # Draw set of  crosshairs in the centre of the window

    # Horizontal line
    pygame.draw.line(screen, BLACK, (WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2), (WINDOW_WIDTH/2 + 100, WINDOW_HEIGHT/2), line_width)
    # Vertical line
    pygame.draw.line(screen, BLACK, (WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 100), (WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 100), line_width)

    # Draws a square
    pygame.draw.rect(screen, SHADOW, (x, y, width, height))

    # Updates Pygame window
    pygame.display.update()


# Main game loop
ser.open() # open communication port
initialMsg = ser.readline() # read initial line (which is usually garbage)
while True:
    # reads serial
    msg = str(ser.readline()).replace("\\r\\n","")
    msg = msg.replace("b'","")
    msg = msg.replace("'","")
    data = msg.split(',')
    second = -int(data[1])

    time = pygame.time.get_ticks() # Keeps track of total runtime

    pygame.time.delay(50)      # Delays the start of the game


    # Quits the game
    for event in pygame.event.get():
        if (event.type == pygame.QUIT):
            print("Time Run:", time / 1000, "seconds")
            pygame.quit()
            sys.exit()

    keys = pygame.key.get_pressed()     # Returns a list of boolean values representing the state of every key on the keyboard
    
    x += second # set position from serial data

    # Updates the square's position for each keystroke event
    if keys[left] and x > vel:
        x -= vel
    if keys[right] and x < WINDOW_WIDTH - width - vel:
        x += vel
    if keys[up] and y > vel:
        y -= vel
    if keys[down] and y < WINDOW_WIDTH - height - vel:
        y += vel

    drawScreen()

ser.close()
