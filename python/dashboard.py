import tkinter as tk
import fileinput
from threading import Thread
from time import sleep
from collections import namedtuple
from collections import defaultdict

# NamedTuple definition
Size = namedtuple("Size", ["width", "height"])
Point = namedtuple("Point", ["x", "y"])
Rect = namedtuple("Rect", ["offset", "size"])
Font = namedtuple("Font", ["family", "size"])

class Dashboard(tk.Tk):
    # Canvas constant
    CANVAS_SIZE = Size(width=200, height=300)
    CANVAS_SCALE = 2.2

    # Font constant
    FONT = Font("Consolas", 18)

    # Color constant
    BG_COLOR = "#0f160d"
    FONT_COLOR = "#e2fcde"
    SHAPE_COLOR = "#162113"

    # Text format constant
    TIRE_TEXT_FORMAT = "%3d\n[%%]"
    LSEN_TEXT_FORMAT = "> LINE_SENSOR: %d, %d, %d"
    DIST_TEXT_FORMAT = "> DISTANCE: %.2f[CM]"
    POS_TEXT_FORMAT = "> POSITION: %f, %f, %f"

    # Rect constant
    BOARD_RECT = Rect(Point(0, 0), Size(100, 200))
    LEFT_TIRE_RECT = Rect(Point(-70, 70), Size(30, 60))
    RIGHT_TIRE_RECT = Rect(Point(70, 70), Size(30, 60))

    def __init__(self):
        super().__init__()

        # Initialize tkinter
        self.title("JLTC")
        self.geometry("1280x720")
        self.config(bg=self.BG_COLOR)
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=1)

        # Initialize frames
        canvas_frame = tk.LabelFrame(
            self, text="VIEW", bg=self.BG_COLOR, fg=self.FONT_COLOR, font=self.FONT
        )
        canvas_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")
        self.create_canvas(canvas_frame)

        info_frame = tk.LabelFrame(
            self, text="INFO", bg=self.BG_COLOR, fg=self.FONT_COLOR, font=self.FONT
        )
        info_frame.grid(row=0, column=1, padx=10, pady=10, sticky="nsew")
        info_inner_frame = tk.Frame(info_frame, bg=self.BG_COLOR)
        info_inner_frame.pack(side="left", padx=10, pady=10, fill="y")
        self.create_line_sensor_label(info_inner_frame)
        self.create_distance_label(info_inner_frame)
        self.create_position_label(info_inner_frame)

        # Initialize input thread
        Thread(target=self.input_thread).start()

    def create_canvas(self, parent):
        self.canvas = tk.Canvas(
            parent,
            width=self.CANVAS_SIZE.width * self.CANVAS_SCALE,
            height=self.CANVAS_SIZE.height * self.CANVAS_SCALE,
            highlightthickness=0,
            bg=self.BG_COLOR
        )
        self.create_canvas_elements()
        self.canvas.scale(tk.ALL, 0, 0, self.CANVAS_SCALE, self.CANVAS_SCALE)
        self.canvas.pack()

    def create_canvas_elements(self):
        self.board = self.create_board(self.BOARD_RECT)
        self.left_tire, self.left_tire_text = self.create_tire(self.LEFT_TIRE_RECT)
        self.right_tire, self.right_tire_text = self.create_tire(self.RIGHT_TIRE_RECT)

    def create_board(self, rect):
        (x0, y0), (x1, y1) = self._rect_to_points(rect)
        board = self.canvas.create_rectangle(
            x0, y0, x1, y1, fill=self.SHAPE_COLOR, outline=""
        )
        return board

    def create_tire(self, rect):
        (x0, y0), (x1, y1) = self._rect_to_points(rect)
        tire = self.canvas.create_rectangle(
            x0, y0, x1, y1, fill=self.SHAPE_COLOR, outline=""
        )
        tire_text = self.canvas.create_text(
            (x0 + x1) / 2,
            (y0 + y1) / 2,
            text=self.TIRE_TEXT_FORMAT % 0,
            font=self.FONT,
            fill=self.FONT_COLOR
        )
        return (tire, tire_text)

    def _rect_to_points(self, rect):
        canvas_mid = Point(x=self.CANVAS_SIZE.width / 2, y=self.CANVAS_SIZE.height / 2)
        rect_mid = Point(x=rect.size.width / 2, y=rect.size.height / 2)
        return [
            Point(
                canvas_mid.x - rect_mid.x + rect.offset.x,
                canvas_mid.y - rect_mid.y + rect.offset.y
            ),
            Point(
                canvas_mid.x + rect_mid.x + rect.offset.x,
                canvas_mid.y + rect_mid.y + rect.offset.y
            )
        ]

    def create_line_sensor_label(self, parent):
        self.lsen_label = tk.Label(
            parent,
            text=self.LSEN_TEXT_FORMAT % (0, 0, 0),
            bg=self.BG_COLOR,
            fg=self.FONT_COLOR,
            font=self.FONT
        )
        self.lsen_label.pack(anchor="w")

    def create_distance_label(self, parent):
        self.dist_label = tk.Label(
            parent,
            text=self.DIST_TEXT_FORMAT % 0,
            bg=self.BG_COLOR,
            fg=self.FONT_COLOR,
            font=self.FONT
        )
        self.dist_label.pack(anchor="w")

    def create_position_label(self, parent):
        self.pos_label = tk.Label(
            parent,
            text=self.POS_TEXT_FORMAT % (0, 0, 0),
            bg=self.BG_COLOR,
            fg=self.FONT_COLOR,
            font=self.FONT
        )
        self.pos_label.pack(anchor="w")

    # Input monitoring from standard input
    def input_thread(self):
        command_handlers = defaultdict(lambda: lambda args: None, {
            "setMotor": self.update_set_motor,
            "getLineSensor": self.update_get_line_sensor,
            "nearDistanceWarningThread": self.update_near_distance_warning_thread,
            "locationManagementThread": self.update_location_management_thread
        })
        for line in fileinput.input():
            _, command, *args = [s.strip() for s in line.split(",")]
            command_handlers[command](args)
            sleep(0.001)

    def update_set_motor(self, args):
        left, right = [int(float(arg) * 100) for arg in args]
        left_text = self.TIRE_TEXT_FORMAT % left
        right_text = self.TIRE_TEXT_FORMAT % right
        self.canvas.itemconfig(self.left_tire_text, text=left_text)
        self.canvas.itemconfig(self.right_tire_text, text=right_text)

    def update_get_line_sensor(self, args):
        lsen = tuple(map(int, args))
        self.lsen_label.config(text=self.LSEN_TEXT_FORMAT % lsen)

    def update_near_distance_warning_thread(self, args):
        dist = float(args[0])
        self.dist_label.config(text=self.DIST_TEXT_FORMAT % dist)

    def update_location_management_thread(self, args):
        pos = tuple(map(float, args))
        self.pos_label.config(text=self.POS_TEXT_FORMAT % pos)

if __name__ == "__main__":
    app = Dashboard()
    app.mainloop()
