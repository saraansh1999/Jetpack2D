#include "main.h"
#include "timer.h"
#include "ball.h"
#include "floor.h"
#include "background.h"
#include "player.h"
#include "coins.h"
#include "fireline.h"
#include "balloon.h"
#include "moving_fire.h"
#include "magnet.h"
#include "boomerang.h"
#include "powerup.h"
#include "ring.h"
#include "viserion.h"
#include "iceball.h"
#include "segment_display.h"
#include "propulsion.h"
#include "collisions.h"
#include "meta.h"

#define C 200
#define F 50
#define MF 50 //even
#define P 20
#define R 20

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

float width  = 800;
float height = 600;
float left_edge = 0;
float right_edge = left_edge + width;
float bottom;
float tot = 80000;
float lev2 = 3000;
float lev3 = 10000;
int cur_level = 1;
double curFrame=0, lastFrame=0;
double delta_time;
bool magnet_present = false;
bool boom_present = false;
bool viserion_present = false;
bool in_ring = false;
bool safe = false;
Timer balloon_timer(0.2);
Timer propulsion_timer(0.1);
Timer magnet_timer_gap(30);
Timer magnet_lifetime(3);
Timer boom_timer(20);
Timer viserion_timer_gap(40);
Timer viserion_lifetime(10);
Timer iceball_timer(2);

/**************************
* Customizable functions *
**************************/

Meta meta;
CollisionDetector Detector;
Floor ground;
Back wall;
Player player;
Magnet magnet;
Boomerang boom;
map<int, Coins> coins;
map<int, Fireline> firelines;
map<int, Moving_fire> moving_fires; 
vector<Balloons> balloons;
vector<Prop> props;
vector<Powerup> powerups;
vector<Ring> rings;
vector<Iceball> iceballs;
Ring active_ring;
Viserion viserion;
Segment num_coins[3];
Segment lives_left;
Segment L;
Segment levno;
 
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 80);

void playerHit()
{
    meta.hit(window);
    player.set_position(screen_center_x*width, -height + ground.box.height + player.box.height/2);
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 0.0f, 0.0f, 0.1f);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane
    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!

    if(player.box.x >= (screen_center_x)*width - width/3)
    {
        screen_center_x += 7/width;
        reset_screen();
    }

    if(player.box.x + player.box.width >= (screen_center_x + 1)*width)
    {
        screen_center_x += 0.5;
        reset_screen();
    }

    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    wall.draw(VP);
    ground.draw(VP);
    for(int i=0;i<props.size();i++)
        props[i].draw(VP);
    player.draw(VP);

    if(cur_level >= 2 && magnet_timer_gap.processTick())
    {
        magnet_present = true;
        magnet.set_position(screen_center_x*width - width/3 + magnet.box.width + rand()%(int)(width/3 + 1), bottom + magnet.box.height + rand()%(int)(height - magnet.box.height + 1));
        magnet_lifetime.reset(3 + rand()%6);
    }
    if(magnet_present)
    {
        if(magnet_lifetime.processTick())
        {
            magnet_present = false;
        }
        else
        {
            magnet.draw(VP);
        }
    }

    if(cur_level == 3 && viserion_timer_gap.processTick())
    {
        viserion_present = true;
        viserion.set_position(screen_center_x*width + width, player.box.y);
        viserion_lifetime.reset(10 + rand()%11);
        iceball_timer.reset(2);
    }
    if(viserion_present)
    {
        if(viserion_lifetime.processTick())
        {
            viserion_present = false;
        }
        else
        {
            viserion.draw(VP);
        }
    }

    if(boom_timer.processTick())
    {
        boom_present = true;
        boom.set_position(screen_center_x*width + width, screen_center_y + height);
    }
    if(boom_present)
    {
        boom.draw(VP);
    }

    for(map<int, Coins>::iterator i=coins.begin();i!=coins.end();i++)
        i->second.draw(VP);
    for(map<int, Fireline>::iterator i=firelines.begin();i!=firelines.end();i++)
        i->second.draw(VP);
    for(map<int, Moving_fire>::iterator i=moving_fires.begin();i!=moving_fires.end();i++)
        i->second.draw(VP);
    for(int i=0;i<balloons.size();i++)
        balloons[i].draw(VP);
    for(int i=0;i<powerups.size();i++)
        powerups[i].draw(VP);
    for(int i=0;i<R;i++)
        rings[i].draw(VP);
    for(int i=0;i<iceballs.size();i++)
        iceballs[i].draw(VP);

    num_coins[0].draw(VP, meta.get_coins()%10);
    num_coins[1].draw(VP, (meta.get_coins()/10)%10);
    num_coins[2].draw(VP, (meta.get_coins()/100)%10);
    lives_left.draw(VP, meta.get_lives());
    L.draw(VP, 10);
    levno.draw(VP, cur_level);
}

void tick_input(GLFWwindow *window) {
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int zee = glfwGetKey(window, GLFW_KEY_Z);
    if(!safe)
    {
        if (right==GLFW_PRESS) {
            player.hor_move(1);
        }
        if(left==GLFW_PRESS){
            player.hor_move(-1);
        }
        if(up==GLFW_PRESS){
            player.ver_move(1);
            if(propulsion_timer.processTick())
            {
                props.push_back(Prop(player.box.x - player.box.width/2, player.box.y - player.box.height/2, COLOR_WHITE));
                props.push_back(Prop(player.box.x + player.box.width/2, player.box.y - player.box.height/2, COLOR_WHITE));
                //props.push_back(Prop(player.box.x, player.box.y - player.box.height/2, COLOR_BLUE));
            }
        }
        if(up!=GLFW_PRESS){
            player.ver_move(-1);
        }
    }
    else
    {
        if (right==GLFW_PRESS) {
            player.hor_move(active_ring.box.x, active_ring.box.y, active_ring.radius, 1);
        }
        if(left==GLFW_PRESS){
            player.hor_move(active_ring.box.x, active_ring.box.y, active_ring.radius, -1);
        }
    }
    if(zee == GLFW_PRESS && balloon_timer.processTick()){
        balloons.push_back(Balloons(player.box.x + player.box.width/2, player.box.y + player.box.height/2, COLOR_BLUE));
    }

}

void tick_elements() {
    //collectibles
    if(player.box.x > tot)
    {
        printf("\n Coins collected: %d\nLives left: %d\n==========You Won!!===========\n", meta.get_coins(), meta.get_lives());
        quit(window);
    }
    for(int i=0;i<powerups.size();)
    {
        if(powerups[i].tick(screen_center_x*width - width, screen_center_x*width + width) && Detector.normal_collision(powerups[i].box, player.box))
        {
            meta.powerup_collect(powerups[i].type);
            powerups.erase(i+powerups.begin());
        }
        else
            i++;
    }
    for(map<int, Coins>::iterator i=coins.begin(), next = i;i!=coins.end();i = next)
    {
        next++;
        i->second.tick();
        if(Detector.normal_collision(i->second.box, player.box))
        {
            meta.coin_collect(i->second.type+1);
            coins.erase(i);
        }
    }
    //ring
    int iterator;
    for(iterator=0;iterator<R;iterator++)
    {
        if(Detector.semicircular_collision(rings[iterator].box, player.box, 10))
        {
            player.set_position(player.box.x, player.box.y);
            //player.tick(rings[iterator].box.x, rings[iterator].box.y, rings[iterator].radius);
            safe = true;
            active_ring = rings[iterator];
            break;
        }                   
    }
    if(iterator==R)
    {
        safe = false;
    }
    if(!safe)
    {
        if(magnet_present)
        {
            magnet.tick();
            player.tick(magnet.box.x, magnet.box.y);
        }
        else
            player.tick(0);
    }
    else    
        player.tick(1);
    //enemies
    if(viserion_present)
    {
        viserion.tick(screen_center_x*width + width, player.box.y);
        if(iceball_timer.processTick())
        {
            iceballs.push_back(Iceball(viserion.position[0], viserion.position[1], COLOR_BLACK));
        }
    }
    if(boom_present)
    {
        boom.tick();
        if(Detector.normal_collision(ground.box, boom.box))
            boom_present = false;
        if(!safe && Detector.normal_collision(boom.box, player.box))
        {
            boom_present = false;
            playerHit();
        }
    }
    for(map<int, Fireline>::iterator i=firelines.begin(), next = i;i!=firelines.end();i=next)
    {
        next++;
        i->second.tick();
        if(!safe && Detector.inclined_collision(i->second.box, i->second.angle, player.box, 2))
        {
            playerHit();
        }
    }
    for(map<int, Moving_fire>::iterator i=moving_fires.begin(), next = i;i!=moving_fires.end();i=next)
    {
        next++;
        i->second.tick();
        if(!safe && Detector.normal_collision(i->second.box,  player.box))
        {
            playerHit();
        }
    }
    for(int i=0;i<iceballs.size();i++)
    {
        iceballs[i].tick();
        if(!safe && Detector.normal_collision(iceballs[i].box, player.box))
        {
            playerHit();
            iceballs.erase(i+iceballs.begin());
        }
        if(iceballs[i].box.x - iceballs[i].box.width/2 <= screen_center_x*width - width)
        {
            iceballs.erase(i+iceballs.begin());
        }
    }

    //balloons
    for(int i=0;i<balloons.size();)
    {
        balloons[i].tick();
        if(Detector.aa_ver_collision(ground.box, balloons[i].box))
            balloons.erase(i+balloons.begin());
        else 
        {
            int flag=0;
            for(map<int, Fireline>::iterator j=firelines.begin(), next = j;j!=firelines.end();j=next)
            {
                next++;
                if(Detector.inclined_collision(j->second.box, j->second.angle, balloons[i].box, 1))
                {
                    firelines.erase(j);
                    balloons.erase(i+balloons.begin());
                    flag++;
                    break;
                }
            }
            for(map<int, Moving_fire>::iterator j=moving_fires.begin(), next = j;j!=moving_fires.end();j=next)
            {
                next++;
                if(Detector.normal_collision(j->second.box, balloons[i].box))
                {
                    moving_fires.erase(j);
                    balloons.erase(i+balloons.begin());
                    flag++;
                    break;
                }
            }
            if(!flag)
                i++;
        }
    }
    //propulsion
    for(int i=0;i<props.size();)
    {
        props[i].tick();
        if(Detector.aa_ver_collision(props[i].box, ground.box))
            props.erase(i+props.begin());
        else
            i++;
    }
    //ground
    ground.tick();
    player.on_ground = Detector.aa_ver_collision(ground.box, player.box);
    if(player.on_ground)
        player.set_position(player.box.x, player.box.y);
    //scoreboard
    num_coins[0].set_position(screen_center_x*width - width + 3.3*num_coins[0].box.width, height - num_coins[0].box.height);
    num_coins[1].set_position(screen_center_x*width - width + 2.2*num_coins[0].box.width, height - num_coins[0].box.height);
    num_coins[2].set_position(screen_center_x*width - width + 1*num_coins[0].box.width, height - num_coins[0].box.height);
    lives_left.set_position(screen_center_x*width - width + 1*lives_left.box.width, height - 2*lives_left.box.height);
    L.set_position(screen_center_x*width - width + 1*lives_left.box.width, height - 3*L.box.height);
    levno.set_position(screen_center_x*width - width + 2.2*lives_left.box.width, height - 3*levno.box.height);
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    meta = Meta();
    wall = Back(0, 0, COLOR_GREY);
    ground = Floor(0, 0, COLOR_WEIRD_DARK_BLUE);
    ground.set_position(0, ground.box.height/2 - height);
    bottom = ground.position[1] + ground.box.height;
    player = Player(0, 0, COLOR_GREEN);
    player.set_position(-400, ground.position[1] + ground.box.height/2 + player.box.height/2);
    magnet = Magnet(0, 0, COLOR_RED);
    boom = Boomerang(0, 0, COLOR_BLACK);
    viserion = Viserion(0, 0, COLOR_BLACK);
    for(int i=0;i<C;i++)
    {
        int type = rand()%2;
        if(type == 1)
            coins.insert(make_pair(i, Coins(0, 0, COLOR_YELLOW, 1)));
        else
            coins.insert(make_pair(i, Coins(0, 0, COLOR_DARK_RED, 0)));
        coins[i].set_position(-width + rand()%(int(width + tot - coins[i].radius)), bottom + rand()%(int(height - bottom - coins[i].radius)));
    }
    for(int i=0;i<F;i++)
    {
        firelines.insert(make_pair(i, Fireline(0, 0, rand()%91, COLOR_RED)));
        firelines[i].set_position(-width + firelines[i].box.width + rand()%(int(width + tot - 2*firelines[i].box.width)), bottom + firelines[i].box.width + rand()%(int(height - bottom - 2*firelines[i].box.width)));
    }
     for(int i=0;i<MF;i++)
    {
        moving_fires.insert(make_pair(i, Moving_fire(0, 0, bottom, height, COLOR_RED, 1)));
        float posx = lev2 + width + rand()%(int(tot - width));
        //float posy = bottom + moving_fires[i].box.height + rand()%(int(height - bottom - 2*moving_fires[i].box.height));
        moving_fires[i].set_position(posx, bottom + 2*moving_fires[i].box.height);
        i++;
        moving_fires.insert(make_pair(i, Moving_fire(0, 0, bottom, height, COLOR_RED, -1)));
        moving_fires[i].set_position(posx, height - 2*moving_fires[i].box.height);
    }
    for(int i=0;i<P;i++)
    {
        int type = rand()%2;
        if(type == 1)
            powerups.push_back(Powerup(0, 0, COLOR_DARK_GREEN, 1));
        else
            powerups.push_back(Powerup(0, 0, COLOR_DARK_YELLOW, 0));
        powerups[i].set_position(width + rand()%(int)(tot - 2*width), 0);
    }
    for(int i=0;i<R;i++)
    {
        rings.push_back(Ring(0, 0, COLOR_BLACK));
        rings[i].set_position(width + rand()%(int)(tot - 2*width), 0);
    }
    num_coins[0] = Segment(0, 0, COLOR_RED);
    num_coins[1] = Segment(0, 0, COLOR_RED);
    num_coins[2] = Segment(0, 0, COLOR_RED);
    lives_left = Segment(0, 0, COLOR_RED);
    L = Segment(0, 0, COLOR_RED);
    levno = Segment(0, 0, COLOR_RED);


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        if (t60.processTick()) {
            // 60 fps
            curFrame = glfwGetTime();
            delta_time = curFrame - lastFrame;
            lastFrame = curFrame;
            if(screen_center_x*width > lev2 && screen_center_x*width <= lev3)
            {
                cur_level = 2;
            }
            else if(screen_center_x*width > lev3)
            {
                cur_level = 3;
            }
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

void reset_screen() {
    float top    = (screen_center_y + 1 / screen_zoom)*height;
    float bottom = (screen_center_y - 1 / screen_zoom)*height;
    float left   = (screen_center_x - 1 / screen_zoom)*width;
    float right  = (screen_center_x + 1 / screen_zoom)*width;
    Matrices.projection = glm::ortho(left, right, bottom, top, -10.0f, 10.0f);
}
