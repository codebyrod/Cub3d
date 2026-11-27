*This project has been created as part of the 42 curriculum by rosousa-, febranda.*

# cub3D

> My first RayCaster with miniLibX

## Description

**cub3D** is a "realistic" 3D first-person view of the inside of a maze, built from scratch in C with the **miniLibX** graphics library. It is inspired by *Wolfenstein 3D* (id Software, 1992), widely considered the first FPS ever made, and uses the **ray-casting** technique to turn a flat 2D map into a 3D scene.

The goal of the project is to practice rigor in C, memory management, basic algorithms, event handling and graphics programming, while using mathematics (vectors, trigonometry, grid traversal) as a practical tool.

### Overview

The program reads a scene description file (`.cub`), validates it, and opens a window where the player can walk through the maze. For every vertical column of the screen, a ray is cast from the player's position through the map grid until it hits a wall. The distance to that wall defines how tall the wall slice is drawn, and the face that was hit (North, South, East or West) defines which texture is applied.

### Features

- Ray-casting engine using the **DDA** (Digital Differential Analyzer) algorithm.
- Fisheye correction through perpendicular wall distance.
- Textured walls, with a different texture for each cardinal direction (N, S, E, W).
- Configurable floor and ceiling colors (RGB).
- Smooth movement independent of the frame rate (delta-time based), with normalized diagonal speed.
- Wall collision.
- Strict `.cub` parser with explicit error messages (`Error\n` + message).
- Clean exit with no memory leaks (ESC key or window close button).

## Instructions

### Requirements

- Linux with an X11 server
- `cc` (gcc or clang) and `make`
- X11 development libraries required by miniLibX

On Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install build-essential xorg libxext-dev zlib1g-dev libbsd-dev
```

The sources of `libft` and `minilibx-linux` are already included in the repository, and the `Makefile` builds both automatically.

### Compilation

From the root of the repository:

```bash
make
```

Available rules:

| Rule         | Description                                              |
|--------------|----------------------------------------------------------|
| `make`/`all` | Builds `libft`, `miniLibX` and the `cub3D` executable    |
| `make clean` | Removes object files (including libft and miniLibX ones) |
| `make fclean`| Removes object files and the `cub3D` executable          |
| `make re`    | Rebuilds everything from scratch                         |

The project is compiled with `cc -Wall -Wextra -Werror`.

### Execution

```bash
./cub3D <path_to_map>.cub
```

Example:

```bash
./cub3D maps/valid/subject.cub
```

> Texture paths inside the `.cub` file are relative to the directory where the program is executed, so run it from the root of the repository when using the provided maps.

### Controls

| Key                  | Action                         |
|----------------------|--------------------------------|
| `W`                  | Move forward                   |
| `S`                  | Move backward                  |
| `A`                  | Strafe left                    |
| `D`                  | Strafe right                   |
| `←` / `→`            | Rotate the view left / right   |
| `ESC`                | Quit the program               |
| Window close button  | Quit the program               |

### Scene description file (`.cub`)

The file must have the `.cub` extension and contain six configuration elements followed by the map, which must always be the **last** element. Configuration elements can appear in any order, be separated by empty lines, and have their values separated by one or more spaces.

| Identifier | Meaning                | Format                          |
|------------|------------------------|---------------------------------|
| `NO`       | North wall texture     | `NO ./path/to/north.xpm`        |
| `SO`       | South wall texture     | `SO ./path/to/south.xpm`        |
| `WE`       | West wall texture      | `WE ./path/to/west.xpm`         |
| `EA`       | East wall texture      | `EA ./path/to/east.xpm`         |
| `F`        | Floor color            | `F R,G,B` (each value in 0–255) |
| `C`        | Ceiling color          | `C R,G,B` (each value in 0–255) |

Textures must be in **`.xpm`** format.

The map is made of only these characters:

| Character      | Meaning                                               |
|----------------|-------------------------------------------------------|
| `0`            | Empty space                                           |
| `1`            | Wall                                                  |
| `N` `S` `E` `W`| Player start position and initial facing direction    |
| ` ` (space)    | Void outside the playable area (valid in the file)    |

Map rules:

- There must be **exactly one** player start.
- The map must be **closed/surrounded by walls**: no walkable tile may touch the map border or a space.
- The map is parsed exactly as it looks in the file.

Minimal example:

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm

F 220,100,0
C 225,30,0

111111
100101
101001
1100N1
111111
```

### Error handling

If anything in the file is misconfigured (wrong arguments or extension, unreadable file, missing/invalid texture, invalid color, invalid character, open map, no or multiple players, etc.), the program prints `Error` followed by an explicit message on the standard error, frees everything it allocated and exits without opening a window.

### Project structure

```
.
├── Makefile
├── main.c               # Entry point: parse, bridge data to the game, start
├── includes/
│   ├── cub3d.h          # Game structures and prototypes
│   └── parser.h         # Parser structures and prototypes
├── parsing/             # .cub parsing and validation
│   ├── parse.c / parse_config.c / parse_map.c
│   ├── parse_values.c   # RGB parsing and texture file checks
│   ├── validation_map.c / validation_size.c
│   └── sufix_verification.c   # .cub / .xpm extension checks
├── game/                # Engine
│   ├── cub3d.c / data_init.c / config_bridge_game.c
│   ├── events.c         # Keyboard and window hooks
│   ├── movement.c       # Movement, rotation and collisions
│   ├── camera.c         # Camera plane and per-column ray direction
│   ├── ray.c / ray_utils.c    # DDA ray traversal
│   ├── wall.c / wall_utils.c  # Wall projection and drawing
│   ├── textures.c       # Texture loading and sampling
│   ├── game_render.c    # Frame rendering
│   └── clear.c / clear_aux.c / err_init_cub.c   # Cleanup
├── libft/               # Libft (with get_next_line)
├── minilibx-linux/      # miniLibX sources
├── textures/            # Example .xpm textures
└── maps/                # Example .cub maps
```

### Technical choices

- **Parser and engine are decoupled.** The parser fills a `t_data_pars` structure, and a bridge (`config_bridge_game.c`) copies that data into the `t_game` structure used by the engine, so the raycaster knows nothing about parsing.
- **DDA grid traversal.** Each ray steps from one grid line to the next (`side_dist` / `delta_dist`), always moving along the closest axis until a wall is found. The axis crossed last tells which face (N/S/E/W) was hit.
- **Camera model.** The player has a direction vector and a camera plane vector (FOV of 66°). Rotating the view rotates both vectors.
- **Perpendicular distance.** Wall height is computed from the distance projected onto the view direction, which avoids the fisheye effect.
- **Image buffer.** Each frame is drawn into a miniLibX image (pixel writes directly into its buffer) and pushed to the window once, instead of drawing pixel by pixel on the window.
- **Frame-rate independent movement.** Speeds are multiplied by the time elapsed between frames (`gettimeofday`). Movement vectors are normalized so pressing two keys at once does not make the player faster.
- **Axis-separated collision.** X and Y movements are tested independently, which lets the player slide along walls.

## Resources

### Documentation and tutorials

- [miniLibX documentation (42 Docs)](https://harm-smits.github.io/42docs/libs/minilibx/): complete guide to the miniLibX functions.
- [Lode's Computer Graphics Tutorial — Raycasting](https://lodev.org/cgtutor/raycasting.html): classic reference for the ray-casting and DDA algorithm.
- [Ray casting — Wikipedia](https://en.wikipedia.org/wiki/Ray_casting) and [Wolfenstein 3D — Wikipedia](https://en.wikipedia.org/wiki/Wolfenstein_3D): background on the technique and the original game.
- [Wolfenstein 3D (playable online)](http://users.atw.hu/wolf3d/): the original game, as suggested by the subject.
- `man 3 math`, `man gettimeofday`, `man 2 open`/`read`/`close`.

### Use of AI

AI tools were used as learning and support tools, and every piece of generated content was reviewed, tested and discussed between the team members before being kept.

- **Claude** (main assistant) and **ChatGPT** (mostly for Git/Git-flow questions).
- **Planning:** helped build a day-by-day project plan, split into verifiable tasks and sprints, with deliverables at the end of each sprint.
- **Math concepts:** helped explain the vector math behind the diagonal movement bug. Pressing two perpendicular keys at once made the player move faster along the diagonal. The fix was to sum the active movement deltas, compute the magnitude of the resulting vector and rescale it to the maximum speed.
- **Debugging:** helped interpret Valgrind reports and reason about pointer/ownership problems and cleanup of partially initialized states.
- **Best practices:** suggestions on code organization and on using integer flags instead of string comparisons inside the render loop.
- **Documentation:** helped structure and write this README.