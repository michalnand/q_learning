#include "visualisation_gl.h"


struct sVisualisation g_visualisation;

void visualisation_paint_cube(float x, float y, float z, float a, float b, float c, float angle, float x0, float y0, u32 cube)
{
    float ax, ay;
    float bx, by;
    float cx, cy;
    float dx, dy;

    rotate_point(&ax, &ay, -a/2.0, b/2.0, angle);
    rotate_point(&bx, &by, a/2.0, b/2.0, angle);
    rotate_point(&cx, &cy, a/2.0, -b/2.0, angle);
    rotate_point(&dx, &dy, -a/2.0, -b/2.0, angle);

    float z_pos_down = 0.0;
    float z_pos_up = c;


    float x_ = x0 + (x - x0)*cos(angle) + (y - y0)*sin(angle);
    float y_ = y0 + (y - y0)*cos(angle) + (x - x0)*sin(angle);

    /*
    float x_ = x;
    float y_ = y;
    */

    if (cube == 0)
    {
        z_pos_down = -0.01;
    }

    glBegin(GL_QUADS);
        glVertex3f(ax + x_, ay + y_, z_pos_down + z);
        glVertex3f(bx + x_, by + y_, z_pos_down + z);
        glVertex3f(cx + x_, cy + y_, z_pos_down + z);
        glVertex3f(dx + x_, dy + y_, z_pos_down + z);

        #ifdef VISUALISATION_3D_MODE

        if (cube != 0)
        {
            glVertex3f(ax + x_, ay + y_, z_pos_up + z);
            glVertex3f(bx + x_, by + y_, z_pos_up + z);
            glVertex3f(cx + x_, cy + y_, z_pos_up + z);
            glVertex3f(dx + x_, dy + y_, z_pos_up + z);

            glVertex3f(bx + x_, by + y_, z_pos_up + z);
            glVertex3f(bx + x_, by + y_, z_pos_down + z);
            glVertex3f(cx + x_, cy + y_, z_pos_down + z);
            glVertex3f(cx + x_, cy + y_, z_pos_up + z);


            glVertex3f(ax + x_, ay + y_, z_pos_up + z);
            glVertex3f(ax + x_, ay + y_, z_pos_down + z);
            glVertex3f(dx + x_, dy + y_, z_pos_down + z);
            glVertex3f(dx + x_, dy + y_, z_pos_up + z);

            glVertex3f(cx + x_, cy + y_, z_pos_down + z);
            glVertex3f(dx + x_, dy + y_, z_pos_down + z);
            glVertex3f(dx + x_, dy + y_, z_pos_up + z);
            glVertex3f(cx + x_, cy + y_, z_pos_up + z);


            glVertex3f(ax + x_, ay + y_, z_pos_down + z);
            glVertex3f(bx + x_, by + y_, z_pos_down + z);
            glVertex3f(bx + x_, by + y_, z_pos_up + z);
            glVertex3f(ax + x_, ay + y_, z_pos_up + z);
        }
        #endif
    glEnd();
}


void visualisation_paint_robot(struct sRobot robot, float base_size)
{
    float x_pos = 0.0;
    float y_pos = 0.0;
    float z_pos = 0.0;

    float ratio = 1.0;


    if (ROBOT_SPACE_DIMENSION > 0)
        x_pos = (robot.position[0]*base_size);

    if (ROBOT_SPACE_DIMENSION > 1)
        y_pos = -(robot.position[1]*base_size);

    if (ROBOT_SPACE_DIMENSION > 2)
        z_pos = (robot.position[2]*base_size);


    switch (robot.type)
    {
        case ROBOT_TYPE_WALL: glColor3f(COLOR_WALL_R/256.0, COLOR_WALL_G/256.0, COLOR_WALL_B/256.0); break;
        case ROBOT_TYPE_COMMON : glColor3f(COLOR_GREEN_ROBOT_R/256.0, COLOR_GREEN_ROBOT_G/256.0, COLOR_GREEN_ROBOT_B/256.0); break;

        // case ROBOT_TYPE_PATH: glColor3f(COLOR_PATH_R/256.0, COLOR_PATH_G/256.0, COLOR_PATH_B/256.0); break;
        case ROBOT_TYPE_TARGET: glColor3f(COLOR_TARGET_R/256.0, COLOR_TARGET_G/256.0, COLOR_TARGET_B/256.0); break;
        case ROBOT_TYPE_SOURCE: glColor3f(COLOR_SOURCE_R/256.0, COLOR_SOURCE_G/256.0, COLOR_SOURCE_B/256.0); break;
        case ROBOT_TYPE_DESTINATION: glColor3f(COLOR_DESTINATION_R/256.0, COLOR_DESTINATION_G/256.0, COLOR_DESTINATION_B/256.0); break;

        case ROBOT_TYPE_RED_ROBOT: glColor3f(COLOR_RED_ROBOT_R/256.0, COLOR_RED_ROBOT_G/256.0, COLOR_RED_ROBOT_B/256.0); break;
        case ROBOT_TYPE_RED_TARGET: glColor3f(COLOR_RED_TARGET_R/256.0, COLOR_RED_TARGET_G/256.0, COLOR_RED_TARGET_B/256.0); break;
        case ROBOT_TYPE_RED_PATH: glColor3f(COLOR_RED_PATH_R/256.0, COLOR_RED_PATH_G/256.0, COLOR_RED_PATH_B/256.0); break;

        case ROBOT_TYPE_GREEN_ROBOT: glColor3f(COLOR_GREEN_ROBOT_R/256.0, COLOR_GREEN_ROBOT_G/256.0, COLOR_GREEN_ROBOT_B/256.0); break;
        case ROBOT_TYPE_GREEN_TARGET: glColor3f(COLOR_GREEN_TARGET_R/256.0, COLOR_GREEN_TARGET_G/256.0, COLOR_GREEN_TARGET_B/256.0); break;
        case ROBOT_TYPE_GREEN_PATH: glColor3f(COLOR_GREEN_PATH_R/256.0, COLOR_GREEN_PATH_G/256.0, COLOR_GREEN_PATH_B/256.0); break;

        case ROBOT_TYPE_BLUE_ROBOT: glColor3f(COLOR_BLUE_ROBOT_R/256.0, COLOR_BLUE_ROBOT_G/256.0, COLOR_BLUE_ROBOT_B/256.0); break;
        case ROBOT_TYPE_BLUE_TARGET: glColor3f(COLOR_BLUE_TARGET_R/256.0, COLOR_BLUE_TARGET_G/256.0, COLOR_BLUE_TARGET_B/256.0); break;
        case ROBOT_TYPE_BLUE_PATH: glColor3f(COLOR_BLUE_PATH_R/256.0, COLOR_BLUE_PATH_G/256.0, COLOR_BLUE_PATH_B/256.0); break;


        case ROBOT_TYPE_RED_PHEROMONE:  glColor3f(robot.parameter_f*COLOR_RED_ROBOT_R/256.0, robot.parameter_f*COLOR_RED_ROBOT_G/256.0, robot.parameter_f*COLOR_RED_ROBOT_B/256.0);
                                        ratio = 0.5;
                                        break;

        case ROBOT_TYPE_GREEN_PHEROMONE:  glColor3f(robot.parameter_f*COLOR_GREEN_ROBOT_R/256.0, robot.parameter_f*COLOR_GREEN_ROBOT_G/256.0, robot.parameter_f*COLOR_GREEN_ROBOT_B/256.0);
                                        ratio = 0.5;
                                        break;

        case ROBOT_TYPE_BLUE_PHEROMONE:  glColor3f(robot.parameter_f*COLOR_BLUE_ROBOT_R/256.0, robot.parameter_f*COLOR_BLUE_ROBOT_G/256.0, robot.parameter_f*COLOR_BLUE_ROBOT_B/256.0);
                                        ratio = 0.5;
                                        break;


        case ROBOT_TYPE_PATH:  glColor3f(robot.parameter_f*COLOR_PATH_R/256.0, robot.parameter_f*COLOR_PATH_G/256.0, robot.parameter_f*COLOR_PATH_B/256.0);
                                        //ratio = 0.5;
                                        break;

        default:  glColor3f(1.0, 1.0, 1.0); break;
    }


    u32 cube = 0;

    if ((robot.type&ROBOT_3D_FLAG) != 0)
        cube = 1;

    visualisation_paint_cube(x_pos*base_size*2.0, y_pos*base_size*2.0, z_pos*base_size*2.0,
                             base_size*ratio,
                             base_size*ratio,
                             base_size*ratio,
                             robot.angles[0], x_pos, y_pos, cube);
}


void visualisation_process_keys(unsigned char key, int x, int y)
{
    if ((key == 'p') || (key == 'P'))
        switch (g_visualisation.view_state)
        {
            case 0: g_visualisation.view_state = 1; break;
            case 1: g_visualisation.view_state = 2; break;
            case 2: g_visualisation.view_state = 0; break;

            default: g_visualisation.view_state = 0; break;
        }

    printf("%i\n", g_visualisation.view_state);
}

void visualisation_print(i32 x, i32 y, char *s)
{
    glColor3f(1.0, 1.0, 1.0);

    glRasterPos2f(x, y);

    u32 len, i;
    len = (u32)strlen(s);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }
}

void visualisation_render()
{
    struct sVect3D  camera;
    u32 j, robot_idx = 0;


    glMatrixMode(GL_PROJECTION);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
     glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_MODELVIEW);


    glClearColor(0.0, 0.0, 0.0, 0.0);

    switch (g_visualisation.view_state)
    {
        case 0:
                glTranslatef(0.0, 0.0, -14.0);
                glRotatef(0.0, 1.0, 0.0, 0.0);
                break;

        case 1:
                glTranslatef(0.0, 0.0, 0.0);
                glRotatef(-70.0, 1.0, 0.0, 0.0);
                glRotatef(g_visualisation.angle, 0.0, 0.0, 1.0);
                g_visualisation.angle+= 0.4;
                break;

        case 2:
                glTranslatef(0.0, -1.0, -18.5);
                glRotatef(-70.0, 1.0, 0.0, 0.0);
                glRotatef(g_visualisation.angle, 0.0, 0.0, 1.0);
                g_visualisation.angle+= 0.4;
                break;
    }

    /*
    glTranslatef(0.0, 0.0, -4.5);
    glRotatef(0.0, 1.0, 0.0, 0.0);
    */

    /*
 	glTranslatef(0.0, 0.0, -2.5);
    glRotatef(-70.0, 1.0, 0.0, 0.0);
    */

    glClearColor(0.0, 0.0, 0.0, 0.0);


    struct sRobot robot;
    g_visualisation.mutex.lock();

    for (j = 0; j < g_visualisation.robots.size(); j++)
    {
        robot = g_visualisation.robots[j];
        visualisation_paint_robot(robot, g_visualisation.base_size);
    }

    visualisation_print(g_visualisation.text_x, g_visualisation.text_y, g_visualisation.text_s);

    g_visualisation.mutex.unlock();




	glutSwapBuffers();
}

void visualisation_rendering_thread()
{
	glutMainLoop();
}

void visualisation_idle()
{

}

void visualisation_init()
{
	int num = 0;

    g_visualisation.text_x = -8.0;
    g_visualisation.text_y = 5.0;
    g_visualisation.text_s = (char*)" ";


    g_visualisation.view_state = 0;
    g_visualisation.angle = 0.0;
    glutInit(&num, NULL);
    glutInitWindowSize(g_visualisation.window_width, g_visualisation.window_height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("robot visualise - swarm of hentai artificial intelligence pink fridges");

    #ifdef VISUALISATION_FULL_SCREEN
    glutFullScreen();
    #endif



    glutIdleFunc(visualisation_render);

    glutKeyboardFunc(visualisation_process_keys);

    glViewport(0, 0, g_visualisation.window_width, g_visualisation.window_height);
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST);
    gluPerspective(45, (float) g_visualisation.window_width / g_visualisation.window_height, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);

    g_visualisation.rendering_thread_main_loop = new std::thread(&visualisation_rendering_thread);
}


void visualisation_update(struct sRobot robot)
{
	u32 i;

    g_visualisation.mutex.lock();
	for (i = 0; i < g_visualisation.robots.size(); i++)
		if (g_visualisation.robots[i].id == robot.id)
		{
	   		g_visualisation.robots[i] = robot;
	   		break;
		}

	if (i == g_visualisation.robots.size())
	    g_visualisation.robots.push_back(robot);
    g_visualisation.mutex.unlock();
}


void visualisation_update_all(std::vector<struct sRobot> *robots)
{
    u32 i;

    g_visualisation.mutex.lock();

    if (g_visualisation.robots.size() != robots->size())
        g_visualisation.robots.resize(robots->size());

    for (i = 0; i < g_visualisation.robots.size(); i++)
        g_visualisation.robots[i] = (*robots)[i];

    g_visualisation.mutex.unlock();
}
