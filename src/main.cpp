#include <iostream>
#include <SDL.h>

SDL_GameController *selectGameController();

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Gyro Recorder",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        400,
        225,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GameController *controller = selectGameController();
    if (controller != nullptr) {
        SDL_GameControllerSetSensorEnabled(controller, SDL_SENSOR_ACCEL, SDL_TRUE);
        SDL_GameControllerSetSensorEnabled(controller, SDL_SENSOR_GYRO, SDL_TRUE);
        SDL_GameControllerEventState(SDL_ENABLE);
    }

    float gyroData[3] = { 0.0, 0.0, 0.0 };
    float accelData[3] = { 0.0, 0.0, 0.0 };

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_CONTROLLERSENSORUPDATE:
                    if (e.csensor.sensor == SDL_SENSOR_ACCEL) {
                        accelData[0] = e.csensor.data[0];
                        accelData[1] = e.csensor.data[1];
                        accelData[2] = e.csensor.data[2];
                    } else if (e.csensor.sensor == SDL_SENSOR_GYRO) {
                        gyroData[0] = e.csensor.data[0];
                        gyroData[1] = e.csensor.data[1];
                        gyroData[2] = e.csensor.data[2];
                    }
                    std::cout << e.csensor.timestamp_us << ","
                        << accelData[0] << "," << accelData[1] << "," << accelData[2] << ","
                        << gyroData[0] << "," << gyroData[1] << "," << gyroData[2] << std::endl;
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    if (e.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                        running = false;
                    }
                    break;
                default:
                    break;
            }
        }

        SDL_Delay(8);
    }

    if (controller != nullptr) {
        SDL_GameControllerClose(controller);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

SDL_GameController *selectGameController() {
    int num = SDL_NumJoysticks();

    for (int i = 0; i < num; i++) {
        SDL_GameController *c = SDL_GameControllerOpen(i);
        if (c != nullptr) {
            bool hasGyro = SDL_GameControllerHasSensor(c, SDL_SENSOR_GYRO);
            bool hasAccel = SDL_GameControllerHasSensor(c, SDL_SENSOR_ACCEL);

            if (hasGyro && hasAccel) {
                return c;
            } else {
                SDL_GameControllerClose(c);
            }
        } else {
            std::cerr << "Failed to open controller number " << i << ", " << SDL_GetError()
                << std::endl;
        }
    }

    return nullptr;
}