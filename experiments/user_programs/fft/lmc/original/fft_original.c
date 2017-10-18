// https://rosettacode.org/wiki/Fast_Fourier_transform#C

#include <complex.h>
#include <math.h>
#include <stdio.h>

//#define SIZE 32
#define SIZE 8
double PI;
typedef double complex cplx;

void show(const char *s, cplx buf[])
{
  printf("%s", s);

  int i;
  for (i = 0; i < SIZE; i++)
    if (!cimag(buf[i]))
      printf("%.5f ", creal(buf[i]));
    else
      printf("(%.5f, %.5f) ", creal(buf[i]), cimag(buf[i]));

  printf("\n");
}

void _fft(cplx buf[], cplx out[], int n, int step)
{
  if (step < n) {
    _fft(out, buf, n, step * 2);
    _fft(out + step, buf + step, n, step * 2);

    int i;
    for (i = 0; i < n; i += 2 * step) {
      cplx t = cexp(-I * PI * i / n) * out[i + step];
      buf[i / 2] = out[i] + t;
      buf[(i + n) / 2] = out[i] - t;
    }
  }
}

void fft(cplx buf[], int n)
{
  cplx out[n];
  int i;
  for (i = 0; i < n; i++)
    out[i] = buf[i];

  _fft(buf, out, n, 1);
}

int main()
{
  PI = atan2(1, 1) * 4;
  cplx buf[] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
  // cplx buf[] =
  // {0.887, 1.645,2.005,1.069,1.069,0.69,1.046,1.847,0.808,0.617,0.792,1.384,1.782,0.925,0.751,0.858,0.915,1.006,0.985,0.97,1.075,1.183,1.408,1.575,1.556,1.282,1.06,1.061,1.283,1.701,1.101,0.702};

  show("Data: ", buf);
  fft(buf, SIZE);
  show("FFT : ", buf);

  return 0;
}