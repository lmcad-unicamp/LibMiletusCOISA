import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

import org.apache.edgent.function.Functions;
import org.apache.edgent.function.Supplier;
import org.apache.edgent.providers.direct.DirectProvider;
import org.apache.edgent.topology.TStream;
import org.apache.edgent.topology.Topology;

public class FFT {
    private static String DEFAULT_FIX_IO_CONTENT = "false";
    private static String DEFAULT_QUERY_TYPE = "0";
    private static String DEFAULT_WINDOW_SIZE = "4";
    
    private static Scanner in = new Scanner(System.in);

    private static int WINDOW_SIZE;
    private static int window_count;
    private static Complex[] values;

    private static float PI = (float)Math.PI;

    private static Supplier<Iterable<Float> > queryInput() throws Exception{
        return () -> {
            List<Float> lines = new LinkedList<>();
            try {
                while (FFT.in.hasNext()) {
                    lines.add(Float.valueOf(FFT.in.next()));
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            return lines;
        };
    }

    private static class Complex {
        private float real;
        private float imag;

        public Complex(float real, float imag) {
            this.real = real;
            this.imag = imag;
        }

        public static Complex add(Complex a, Complex b) {
            return new Complex(a.real + b.real, a.imag + b.imag);
        }

        public static Complex sub(Complex a, Complex b) {
            return new Complex(a.real - b.real, a.imag - b.imag);
        }

        public static Complex mult(Complex a, Complex b) {
            return new Complex(a.real * b.real - a.imag * b.imag,
                               a.real * b.imag + a.imag * b.real);
        }

        public static Complex exp(Complex z) {
            float imag_sin, imag_cos, real_exp;

            imag_sin = (float) Math.sin(z.imag);
            imag_cos = (float) Math.cos(z.imag);
            real_exp = (float) Math.exp(z.real);

            return new Complex(real_exp * imag_cos, real_exp * imag_sin);
        }
    }

    private static void show(String s) {
        System.out.print(s);

        for (int i = 0; i < FFT.WINDOW_SIZE; ++i) {
            if (values[i].imag == 0) {
                System.out.print(String.format("%.5f", values[i].real) + " ");
            }
            else {
                System.out.print("(" + String.format("%.5f", values[i].real) + ", " +
                                       String.format("%.5f", values[i].imag) + ") ");
            }
        }

        System.out.println();
    }

    private static void _fft(Complex buf[], Complex out[], int first, int step) {
        if (step < FFT.WINDOW_SIZE) {
            FFT._fft(out, buf, first, step * 2);
            FFT._fft(out, buf, first + step, step * 2);

            for (int i = 0; i < FFT.WINDOW_SIZE; i += 2 * step) {
                Complex z = new Complex(0.0f, - FFT.PI * i / FFT.WINDOW_SIZE);

                Complex t = Complex.exp(z);

                t = Complex.mult(t, out[first + i + step]);

                buf[first + (i / 2)] = Complex.add(out[first + i], t);
                buf[first + ((i + FFT.WINDOW_SIZE) / 2)] = Complex.sub(out[first + i], t);
            }
        }
    }

    private static void fft() {
        Complex out[] = Arrays.copyOf(values, values.length);
     
        FFT._fft(values, out, 0, 1);
    }

    public static void main(String[] args) throws Exception {
        FFT.WINDOW_SIZE = Integer.valueOf(
                System.getProperty("WINDOW_SIZE", FFT.DEFAULT_WINDOW_SIZE));
        FFT.values = new Complex[FFT.WINDOW_SIZE];
        FFT.window_count = 0;

        DirectProvider dp = new DirectProvider();
        Topology top = dp.newTopology();

        TStream<Float> tempReadings;

        if (Boolean.valueOf(System.getProperty("FIX_IO_CONTENT", FFT.DEFAULT_FIX_IO_CONTENT))) {
            tempReadings = top.source(queryInput());
        } else {
            TempSensor sensor = new TempSensor();
            tempReadings = top.poll(sensor, 1, TimeUnit.MILLISECONDS);
        }

        TStream<List<Float> > filteredReadings = tempReadings.last(FFT.WINDOW_SIZE, Functions.unpartitioned()).aggregate((samples, key) -> samples);

        filteredReadings.sink(reading -> {
            FFT.values[FFT.window_count] = new Complex(reading.get(reading.size() - 1), 0.0f);

            FFT.window_count += 1;

            if (FFT.window_count == FFT.WINDOW_SIZE) {
                FFT.show("Data: ");
                FFT.fft();
                FFT.show("FFT : ");

                switch(Integer.valueOf(System.getProperty("QUERY_TYPE", FFT.DEFAULT_QUERY_TYPE))) {
                    case 0:
                        System.exit(0);
                        break;
                    case 1:
                        FFT.window_count = 0;
                        break;
                    default:
                        System.out.println("ERROR: Invalid query type.");
                        break;
                }
            }
        });

        dp.submit(top);
    }
}