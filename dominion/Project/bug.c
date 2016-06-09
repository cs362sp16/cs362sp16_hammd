#define SIZE 20

double mult(double z[], int n)
{
   int i, j;

   i = 0;
   for(j = 0; j < n; j++)
   {
      i = i + j + 1;
      z[i] = z[i] * (z[0] + 1.0);
   }

   return z[n];
}

double copy(double to[], double from[], int count)
{
    int n = (count + 7) / 8;
    switch (count % 8) do {
    case 0: *to++ = *from++;
    case 7: *to++ = *from++;
    case 6: *to++ = *from++;
    case 5: *to++ = *from++;
    case 4: *to++ = *from++;
    case 3: *to++ = *from++;
    case 2: *to++ = *from++;
    case 1: *to++ = *from++;
    } while (--n > 0);

    return mult(to, 2);
}

int main(int argc, char *argv[])
{
    double x[SIZE], y[SIZE];
    double *px = x;

    while (px < x + SIZE)
	      *px++ = (px - x) * (SIZE + 1.0);

    return copy(y, x, SIZE);
}