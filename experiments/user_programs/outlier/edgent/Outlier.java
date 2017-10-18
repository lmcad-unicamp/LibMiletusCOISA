import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

import org.apache.edgent.function.Functions;
import org.apache.edgent.function.Supplier;
import org.apache.edgent.providers.direct.DirectProvider;
import org.apache.edgent.topology.TStream;
import org.apache.edgent.topology.Topology;

public class Outlier {
    private static String DEFAULT_FIX_IO_CONTENT = "false";
    private static String DEFAULT_QUERY_TYPE = "0";
    private static String DEFAULT_WINDOW_SIZE = "10";
    
    private static Scanner in = new Scanner(System.in);

    private static int WINDOW_SIZE;
    private static int window_count;
    private static float[] values;

    private static Supplier<Iterable<Float> > queryInput() throws Exception{
        return () -> {
            List<Float> lines = new LinkedList<>();
            try {
                while (Outlier.in.hasNext()) {
                    lines.add(Float.valueOf(Outlier.in.next()));
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            return lines;
        };
    }

    private static float find_median(int begin, int end) {
        if (end < begin) {
            System.out.println("ERROR: End must be greater than or equal to begin.");
            return 0;
        }

        int size = end - begin + 1;
        int middle = begin + size / 2;

        float median;

        if (size % 2 == 0) {
            median = (values[middle - 1] + values[middle]) / 2.0f;
        } else {
            median = values[middle];
        }

        return median;
    }

    private static void insertion_sort() {
        int i, j;
        float current;

        for (i = 1; i < Outlier.WINDOW_SIZE; ++i) {
            current = values[i];
            j = i - 1;

            while ((j >= 0) && (current < values[j])) {
                values[j + 1] = values[j];
                j = j - 1;
            }

            values[j + 1] = current;
        }
    }

    // Tukey's test
    private static void outlier() {
        if (Outlier.WINDOW_SIZE < 1) {
            System.out.println("ERROR: The window must contain at least one number.");
            return;
        }

        // Sort values
        insertion_sort();

        // Calculate median
        //float q2 = find_median(0, Outlier.WINDOW_SIZE - 1);

        // The following calculations use the definition
        // of quartile that does not include the median

        // Calculate lower quartile
        float q1 = find_median(0, (Outlier.WINDOW_SIZE / 2) - 1);

        // Calculate upper quartile
        float q3;

        if (Outlier.WINDOW_SIZE % 2 == 0) {
            q3 = find_median(Outlier.WINDOW_SIZE / 2, Outlier.WINDOW_SIZE - 1);
        } else {
            q3 = find_median((Outlier.WINDOW_SIZE / 2) + 1, Outlier.WINDOW_SIZE - 1);
        }

        // Find interquartile range
        float range = q3 - q1;

        // Find inner fences
        float inner_lower_boundary = q1 - range * 1.5f;
        float inner_upper_boundary = q3 + range * 1.5f;

        // Find outer fences
        float outer_lower_boundary = q1 - range * 3.0f;
        float outer_upper_boundary = q3 + range * 3.0f;

        /* An outlier is an observation outside of the range
         * [q1 - k * (q3 - q1), q3 + k * (q3 - q1)].
         *
         * It is a minor outlier when k = 1.5 and
         * a major outlier when k = 3.0.
         */

        for (int i = 0; i < Outlier.WINDOW_SIZE; ++i) {
            if (values[i] < outer_lower_boundary) {
                System.out.println("Major: " + String.format("%.5f", values[i]));
            } else {
                if (values[i] < inner_lower_boundary) {
                    System.out.println("Minor: " + String.format("%.5f", values[i]));
                } else {
                    if (values[i] > outer_upper_boundary) {
                        System.out.println("Major: " + String.format("%.5f", values[i]));
                    } else {
                        if (values[i] > inner_upper_boundary) {
                            System.out.println("Minor: " + String.format("%.5f", values[i]));
                        }
                    }
                }
            }
        }
    }

    public static void main(String[] args) throws Exception {
        Outlier.WINDOW_SIZE = Integer.valueOf(
                System.getProperty("WINDOW_SIZE", Outlier.DEFAULT_WINDOW_SIZE));
        Outlier.values = new float[Outlier.WINDOW_SIZE];
        Outlier.window_count = 0;

        DirectProvider dp = new DirectProvider();
        Topology top = dp.newTopology();

        TStream<Float> tempReadings;

        if (Boolean.valueOf(System.getProperty("FIX_IO_CONTENT", Outlier.DEFAULT_FIX_IO_CONTENT))) {
            tempReadings = top.source(queryInput());
        } else {
            TempSensor sensor = new TempSensor();
            tempReadings = top.poll(sensor, 1, TimeUnit.MILLISECONDS);
        }

        TStream<List<Float> > filteredReadings = tempReadings.last(Outlier.WINDOW_SIZE, Functions.unpartitioned()).aggregate((samples, key) -> samples);

        filteredReadings.sink(reading -> {
            Outlier.values[Outlier.window_count] = reading.get(reading.size() - 1);

            Outlier.window_count += 1;

            if (Outlier.window_count == Outlier.WINDOW_SIZE) {
                outlier();

                switch(Integer.valueOf(System.getProperty("QUERY_TYPE", Outlier.DEFAULT_QUERY_TYPE))) {
                    case 0:
                        System.exit(0);
                        break;
                    case 1:
                        Outlier.window_count = 0;
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