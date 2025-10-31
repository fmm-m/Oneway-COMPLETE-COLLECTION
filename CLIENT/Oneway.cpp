// Oneway.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "raylib.h"
#include <cstring>
#include <iostream>
#include <math.h>
#include <format>


#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <WinSock2.h>
#include <ws2tcpip.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif


#include "Car.h"
#include "Vec2.h"
#include "Course.h"
#include "CPU.h"
#include <time.h>
#include "Button.h"

#define SERVERPORT "51283"   // the port users will be connecting to

addrinfo clientSetup(const char DEST[], const char PORT[], int*& sockfdRef) {


    
    
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    std::cout << "Started" << "\n";
    int sockfd;
    struct addrinfo hints, * servinfo, * p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to use IPv4
    hints.ai_socktype = SOCK_DGRAM;

    rv = getaddrinfo(DEST, PORT, &hints, &servinfo);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

    }

    // loop through all the results and make a socket
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");

    }

    connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen); // Connect to server sop taht i dont have to deal with sendto() recvfrom()
    *sockfdRef = sockfd;
    return *servinfo;
}

std::string getuser(std::string info, float* X, float* Y, float* ANGLE) {

    std::string username;
    std::string x;
    std::string y;
    std::string angle;
    int infoSize = info.size();
    std::cout << info[1] << " << INFO 1\n";


    int i = 0;
    if (info[1] == 'P') {
        i += 3;
        std::cout << "WAFAWAFAFW\n";
    }
    
    std::cout << "Info ?" << info[i] << "?\n";
    for (i; i < infoSize; i++) {
        if (info[i] == ':') {
            i++;
            break;
        }
        username.push_back(info[i]);
    }
    for (i; i < infoSize; i++) {
        if (info[i] == ':') {
            i++;
            break;
        }
        x.push_back(info[i]);
    }
    for (i; i < infoSize; i++) {
        if (info[i] == ':') {
            i++;
            break;
        }
        y.push_back(info[i]);
    }
    for (i; i < infoSize; i++) {
        if (info[i] == '\0') {
            i++;
            break;
        }
        angle.push_back(info[i]);
    }
    std::cout << "Angle: " << angle << "\n";
    if (sizeof(x) <= 1 or sizeof(y) <= 1 or sizeof(angle) <= 1) {
        std::cout << "Error thrown by Buffer: " << info << "\n";
        return "ERROR";
    }
    std::cout << "X: -" << x << "- Y: -" << y << "- Angle: -" << angle << "-\n";
  
    *X = std::stof(x);
    *Y = std::stof(y);
    *ANGLE = std::stof(angle);
    return username;

}

int main()
{
    std::string username;
    std::cout << "Please enter Username: ";
    std::getline(std::cin, username);
    std::cout << "Username: " << username << "\n";
    
    std::string addr;
    std::string port;
    
    std::cout << "IP: ";
    std::getline(std::cin, addr);
    
    std::cout << "Port: ";
    std::getline(std::cin, port);

    std::cout << addr << " | " << port << "\n";
    
    const char* DEST = addr.c_str();
    const char* PORT = port.c_str();

    int numbytes;
    int sockfd;
    int* sockfdPtr = &sockfd;
    addrinfo servinfo = clientSetup(DEST, PORT, sockfdPtr);
    int recvBytes;
    char buf[128] = { 0 };

    
    
    int width = 1920;// *1.2;
    int height = 1080;// *1.2;

    InitWindow(width, height, username.c_str());

    InitAudioDevice();
    //ToggleBorderlessWindowed();
    
    width = GetScreenWidth();
    height = GetScreenHeight();

    SetTargetFPS(60);

    int courseLength = 500;
    time_t seed;

    
    Course course = Course(500, courseLength);
    Car car = Car(Vec2(width / 2 + 150.5, height / 2 + 0.5), 0, courseLength);


    seed = course.Generate(width, height);

    Camera2D camera = { 0 };
    camera.target = car.position.toRaylib();
    camera.offset = Vector2(width / 2.0f, height / 2.0f);
    camera.rotation = 0 -car.angle * RAD2DEG - 90;
    camera.zoom = 0.05f;

    time_t startTime;
    time(&startTime);
    time_t currTime;
    time(&currTime);

    int frameCount = 0;
    bool finished = false;

    int winCountdown; // Starts at 400, - 1 each frame to time how long the reset wait is.
    
    Button syncButton = Button(100, 30, "ButtonSprite.jpg", "Sync", Vec2(10, height - 40));


    while (!WindowShouldClose()) {

        syncButton.updateState(Vec2(GetMousePosition()));
        
        if (IsKeyPressed(KEY_SPACE) or finished or IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
            
            finished = false;
            course.maxCorner = Vec2();
            course.minCorner = Vec2();
            seed = course.Generate(width, height);
            
            car.restart(width, height);
            camera.zoom = 0.05f;
            camera.target = car.position.toRaylib();

            

            frameCount = 0;
        }
        if (frameCount == 250) {
            time(&startTime);
        } 
        if (frameCount > 250) {
        
            finished = car.update(course.leftVertices, course.rightVertices);
            
            
            camera.zoom = Lerp(camera.zoom, car.zoom, 0.05);
            //camera.zoom = 0.75;

            camera.rotation = Lerp(camera.rotation, -car.angle * RAD2DEG - 90, 0.05);
            camera.target.x = Lerp(camera.target.x, car.position.x + car.velocity.x * 15, 0.2);
            camera.target.y = Lerp(camera.target.y, car.position.y + car.velocity.y * 15, 0.2);

            time(&currTime);

        }
        else if (frameCount > 50) {
            camera.zoom += 0.01f;
            camera.rotation = Lerp(camera.rotation, -car.angle * RAD2DEG - 90, 0.02);
            frameCount++;
        }
        else {
            frameCount++;
        }

        // NETWORKING STUFF
        std::string msgStr;
        if (not syncButton.justPressed) {
            msgStr = std::format("U:{}:{}:{}:{}", username, roundf(car.position.x), roundf(car.position.y), (float)(roundf(car.angle * 100) / 100));
        }
        else {
            msgStr = std::format("I:{}", course.seed);
        }
        
        char *msg = msgStr.data();
        
        char* msgPtr;
        if ((numbytes = send(sockfd, msg, strlen(msg), 0)) == -1) {
            perror("talker: sendto");
            system("pause");
        }
        
        memset(&buf, 0, sizeof(buf));
        if ((recvBytes = recv(sockfd, buf, 100, 0)) == 0) {
            std::cout << "Received no bytes " << errno; 
            
            
        }
        
        

        // END NETWORKING STUFF
        
        BeginDrawing();
        ClearBackground(DARKGREEN);

        
        
        BeginMode2D(camera);
        
        
        course.Draw(car.checkpoint);
        
        
        

        // PROCESS AND DRAW NETWORKED CARS

        
        std::cout << sizeof(buf) << " : " << buf << "\n";
        switch (buf[0]) {
        case 'P':
            if (sizeof(buf) > 5) {
                //std::cout << buf << "\n";
                std::string rawPlayers;
                for (int i = 2; i < sizeof(buf); i++) {
                    if (buf[i] != '!') {
                        rawPlayers.push_back(buf[i]);
                    }
                }
                //std::cout << rawPlayers << "\n";

                std::string player;
                for (char c : rawPlayers) {
                    if (c != '|') {
                        player.push_back(c);
                    }
                    else {
                        
                        float x;
                        float y;
                        float angle;
                        std::cout << "Player Info: " << player << "\n";
                        std::string otherUsername = getuser(player, &x, &y, &angle);
                        if (otherUsername != "ERROR") {
                            Vec2 position = Vec2(x, y);
                            Vec2 nose = position + Vec2(angle) * 10;
                            DrawCircleV(position.toRaylib(), 12, WHITE);
                            DrawCircleV(nose.toRaylib(), 7, WHITE);
                            DrawCircleV(position.toRaylib(), 10, RED);
                            DrawCircleV(nose.toRaylib(), 5, RED);

                            player = { 0 };
                        }
                        else {
                            std::cout << "Failed to get user info from buffer\n";
                            break;
                        }

                    }
                    if (c == '!') { break; }

                }
            }
            break;
        case 'F':
            std::string winningPlayer;

            for (char c : buf) {
                if (c != ':') {
                    winningPlayer.push_back(c);
                }
            }
            break;
       
        
        }
        
        
        // END NETWORK DRWAIWNG

        car.draw();

        EndMode2D();
        int elapsedTime = difftime(currTime, startTime);
        DrawText(TextFormat("Time: %d", elapsedTime), width - 200, 25, 25, BLACK);
        DrawText(TextFormat("Seed: %d", seed), width - 400, 75, 25, BLACK);
        
        syncButton.draw();
        

        course.DrawMinimap(car);
        
        EndDrawing();
        
    }

    system("pause");
}

