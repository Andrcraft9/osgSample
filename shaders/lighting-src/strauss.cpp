//
// Straus Shading Model.
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	"libExt.h"

#ifdef	MACOSX
	#include	<GLUT/glut.h>
#else
	#include	<glut.h>
#endif

#include	<stdio.h>
#include	<stdlib.h>

#include	"libTexture.h"
#include	"Vector3D.h"
#include	"Vector2D.h"
#include	"Vector4D.h"
#include	"Data.h"
#include	"GlslProgram.h"

Vector3D	eye   ( 7, 5, 7 );			// camera position
Vector3D	light ( 5, 0, 4 );			// light position
float	 	angle = 0;
Vector3D	rot ( 0, 0, 0 );
int			mouseOldX = 0;
int			mouseOldY = 0;

float	smooth = 0.7;
float	metal  = 0.7;
float	transp = 0.1;

GlslProgram	program;

/////////////////////////////////////////////////////////////////////////////////

void init ()
{
	glClearColor ( 0.5, 0.5, 0.5, 1.0 );
	glEnable     ( GL_DEPTH_TEST );
	glDepthFunc  ( GL_LEQUAL );

	glHint ( GL_POLYGON_SMOOTH_HINT,         GL_NICEST );
	glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void display ()
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

												// draw the light
	program.unbind ();

	glMatrixMode ( GL_MODELVIEW );
	glPushMatrix ();

	glTranslatef       ( light.x, light.y, light.z );
	glColor4f          ( 1, 1, 1, 1 );
	glutSolidSphere    ( 0.1f, 15, 15 );
	glPopMatrix        ();

	glMatrixMode   ( GL_MODELVIEW );
	glPushMatrix   ();

    glRotatef    ( rot.x, 1, 0, 0 );
    glRotatef    ( rot.y, 0, 1, 0 );
    glRotatef    ( rot.z, 0, 0, 1 );

	program.bind   ();
//	torus.  draw   ();
	glutSolidTeapot ( 2.5 );
    
	program.unbind ();

    glPopMatrix ();

	glutSwapBuffers ();
}

void reshape ( int w, int h )
{
   glViewport     ( 0, 0, (GLsizei)w, (GLsizei)h );
   glMatrixMode   ( GL_PROJECTION );
   											// factor all camera ops into projection matrix
   glLoadIdentity ();
   gluPerspective ( 60.0, (GLfloat)w/(GLfloat)h, 1.0, 60.0 );
   gluLookAt      ( eye.x, eye.y, eye.z,	// eye
					0, 0, 0,				// center
					0.0, 0.0, 1.0 );		// up

   glMatrixMode   ( GL_MODELVIEW );
   glLoadIdentity ();
}

void motion ( int x, int y )
{
    rot.y -= ((mouseOldY - y) * 180.0f) / 200.0f;
    rot.z -= ((mouseOldX - x) * 180.0f) / 200.0f;
    rot.x  = 0;

    if ( rot.z > 360 )
		rot.z -= 360;

	if ( rot.z < -360 )
		rot.z += 360;

    if ( rot.y > 360 )
		rot.y -= 360;

	if ( rot.y < -360 )
        rot.y += 360;

    mouseOldX = x;
    mouseOldY = y;

	glutPostRedisplay ();
}

void mouse ( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN )
    {
        mouseOldX = x;
        mouseOldY = y;
	}
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
		
	if ( key == '+' && smooth < 1 - 0.1 )
		smooth += 0.1;
		
	if ( key == '-' && smooth > 0.1 )
		smooth -= 0.1;

	if ( key == '*' && metal < 1 - 0.1 )
		metal += 0.1;
		
	if ( key == '/' && metal > 0.1 )
		metal -= 0.1;

	if ( key == ']' && transp < 1 - 0.1 )
		transp += 0.1;
		
	if ( key == '[' && transp > 0.1 )
		transp -= 0.1;
}

void	animate ()
{
	angle  = 0.004f * glutGet ( GLUT_ELAPSED_TIME );

	light.x = 3*cos ( angle );
	light.y = 4*sin ( 1.4 * angle );
	light.z = 4 + 0.5 * sin ( angle / 3 );

	program.bind ();
	program.setUniformVector ( "eyePos",   Vector4D ( eye,   1 ) );
	program.setUniformVector ( "lightPos", Vector4D ( light, 1 ) );
	program.setUniformFloat  ( "smooth",   smooth );
	program.setUniformFloat  ( "metal",    metal );
	program.setUniformFloat  ( "transp",   transp );
	program.unbind ();

	glutPostRedisplay ();
}

int main ( int argc, char * argv [] )
{
								// initialize glut
	glutInit            ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize  ( 500, 500 );


								// create window
	glutCreateWindow ( "Strauss Lighting Model" );

								// register handlers
	glutDisplayFunc  ( display );
	glutReshapeFunc  ( reshape );
	glutKeyboardFunc ( key     );
	glutMouseFunc    ( mouse   );
	glutMotionFunc   ( motion  );
	glutIdleFunc     ( animate );

	init           ();
	initExtensions ();

	assertExtensionsSupported ( "GL_ARB_shading_language_100 GL_ARB_shader_objects" );

	if ( !program.loadShaders ( "strauss.vsh", "strauss.fsh" ) )
	{
		printf ( "Error loading shaders:\n%s\n", program.getLog ().c_str () );

		return 3;
	}

	printf ( "Use + and - keys to change smooth parameter\nUse * and / keys to change metal parameter\n[ and ] keys to change transp parameter\n" );
	
    glutMainLoop ();

	return 0;
}
