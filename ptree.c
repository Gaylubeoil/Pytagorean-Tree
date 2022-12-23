#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif
#define COUNTER_START 0
#define X_START 0
#define Y_START 0
#define ANGLE_START 0

double grad_to_rad(double angle)
{
  return angle * M_PI / 180;
}

double rad_to_grad(double angle)
{
  return angle / (M_PI / 180);
}

void draw(FILE *fp, double x, double y, double length, double rot, double rx, double ry, char *color)
{
  fprintf(fp, "<rect x='%f' y='%f' width='%f' height='%f' transform='rotate(%f %f %f)' style='fill:%s; stroke-width:0;' />\n", x, y, length, length, rad_to_grad(rot), rx, ry, color);
  return;
}

void tree(FILE *fp, const int depth, int counter, double rotation, const double angle, double length, double x, double y, char **palette)
{
  char *color = palette[counter];
  draw(fp, x, y, length, rotation, x, y, color);

  double length_leftq = cos(angle) * length;
  double qleft_x = x - sin(rotation) * length;
  double qleft_y = y + cos(rotation) * length;

  double length_rightq = sin(angle) * length;
  double qright_x = x + cos(angle + rotation) * length_leftq - sin(rotation) * length;
  double qright_y = y + cos(rotation) * length + sin(angle + rotation) * length_leftq;

  if (counter == depth - 1)
    return;

  counter++;

  tree(fp, depth, counter, (rotation + angle), angle, length_leftq, qleft_x, qleft_y, palette);
  tree(fp, depth, counter, (rotation + angle - grad_to_rad(90.0)), angle, length_rightq, qright_x, qright_y, palette);
}

int main(int argn, char *argv[])
{
  int depth;
  double angle, length;

  if (argn == 4)
  {
    length = abs(atof(argv[0]));
    angle = atof(argv[1]);
    depth = atoi(argv[2]);
  }
  else
  {
    printf("Error on argument passing. Setting sample values");
    depth = 13;
    angle = 45.0;
    length = 10.0;
  }

  char *palette[15] = {"red", "blue", "green", "orange", "magenta", "cyan", "yellow", "grey", "black", "purple", "red", "blue", "green", "orange", "pink"};

  FILE *fp;
  fp = fopen("sheet8.svg", "w");
  fprintf(fp, "<?xml version='1.0' encoding='UTF-8'?>\n"
              "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' version='1.1' width='100%%' height='100%%' viewBox='-50 -50 100 100' transform='scale(1 -1)'>\n"
              "  <title>Exampel</title>\n");

  tree(fp, depth, COUNTER_START, ANGLE_START, grad_to_rad(angle), length, X_START, Y_START, palette);

  fprintf(fp, "</svg>\n");
  fclose(fp);

  return 0;
}