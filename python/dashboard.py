import tkinter as tk
import fileinput
import threading
from time import sleep
from collections import namedtuple

Size = namedtuple("Size", ["width", "height"])
Point = namedtuple("Point", ["x", "y"])
Rect = namedtuple("Rect", ["offset", "size"])

def create_round_rectangle(x0, y0, x1, y1, r, **kwargs):
    points = [
        x0 + r, y0, x1 - r, y0, x1, y0, x1, y0 + r, x1, y1 - r, x1, y1,
        x1 - r, y1, x0 + r, y1, x0, y1, x0, y1 - r, x0, y0 + r, x0, y0
    ]
    return canvas.create_polygon(points, **kwargs, smooth = True)

def rect_to_points(rect):
    canvas_mid = Point(x = canvas_size.width / 2, y = canvas_size.height / 2)
    rect_mid = Point(x = rect.size.width / 2, y = rect.size.height / 2)
    return (
        canvas_mid.x - rect_mid.x + rect.offset.x,
        canvas_mid.y - rect_mid.y + rect.offset.y,
        canvas_mid.x + rect_mid.x + rect.offset.x,
        canvas_mid.y + rect_mid.y + rect.offset.y
    )

def input_thread():
    for line in fileinput.input():
        time_str, command, *args = [s.strip() for s in line.split(",")]
        time = int(time_str)
        if (command == "setMotor"):
            set_motor(args)
        sleep(0.01)

def set_motor(args):
    left, right = [int(arg) for arg in args]
    canvas.itemconfig(left_tire_text, text = left)
    canvas.itemconfig(right_tire_text, text = right)

root = tk.Tk()
root.title("JLTC")
root.geometry("1280x720")
root.config(bg = "#212121")

canvas_size = Size(width = 200, height = 300)
canvas_scale = 2.4
canvas = tk.Canvas(
    width = canvas_size.width * canvas_scale,
    height = canvas_size.height * canvas_scale,
    highlightthickness = 0
)
canvas.config(bg = "#212121")

frame_rect = Rect(Point(0, 0), Size(190, 290))
main_board_rect = Rect(Point(0, 0), Size(100, 200))
left_tire_rect = Rect(Point(-65, 70), Size(30, 60))
right_tire_rect = Rect(Point(65, 70), Size(30, 60))
lcd_rect = Rect(Point(0, -50), Size(100, 40))

left_tire_points = rect_to_points(left_tire_rect)
left_tire_mid_point = (
    (left_tire_points[0] + left_tire_points[2]) / 2,
    (left_tire_points[1] + left_tire_points[3]) / 2
)
right_tire_points = rect_to_points(right_tire_rect)
right_tire_mid_point = (
    (right_tire_points[0] + right_tire_points[2]) / 2,
    (right_tire_points[1] + right_tire_points[3]) / 2
)

frame = create_round_rectangle(
    *rect_to_points(frame_rect), r = 5, fill = "#2f2f2f"
)
main_board = create_round_rectangle(
    *rect_to_points(main_board_rect), r = 5, fill = "#b8b8b8"
)
lcd = canvas.create_rectangle(
    rect_to_points(lcd_rect), fill = "#a2c3c9"
)
left_tire = create_round_rectangle(
    *left_tire_points, r = 5, fill = "#000000"
)
left_tire_text = canvas.create_text(
    left_tire_mid_point, text = 0, font = ("Consolas", 18), fill = "#ffffff"
)
right_tire = create_round_rectangle(
    *rect_to_points(right_tire_rect), r = 5, fill = "#000000"
)
right_tire_text = canvas.create_text(
    right_tire_mid_point, text = 0, font = ("Consolas", 18), fill = "#ffffff"
)

canvas.scale(tk.ALL, 0, 0, canvas_scale, canvas_scale)
canvas.place(x = 0, y = 0)

threading.Thread(target = input_thread).start()

root.mainloop()
