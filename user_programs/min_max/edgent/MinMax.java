import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

import org.apache.edgent.analytics.sensors.Range;
import org.apache.edgent.analytics.sensors.Ranges;
import org.apache.edgent.function.Supplier;
import org.apache.edgent.providers.direct.DirectProvider;
import org.apache.edgent.topology.TStream;
import org.apache.edgent.topology.Topology;

public class MinMax {
    private static String DEFAULT_FIX_IO_CONTENT = "false";
    private static String DEFAULT_QUERY_TYPE = "0";
    private static String DEFAULT_RANGE = "[0.0f..100.0f]";

    private static Scanner in = new Scanner(System.in);

    private static Range<Float> values;

    private static Supplier<Iterable<Float> > queryInput() throws Exception{
        return () -> {
            List<Float> lines = new LinkedList<>();
            try {
                while (MinMax.in.hasNext()) {
                    lines.add(Float.valueOf(MinMax.in.next()));
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            return lines;
        };
    }

    public static void main(String[] args) throws Exception {
        // Initialize the range from a Range string in the properties.
        // Use a default value if a range isn't present in the properties.
        MinMax.values = Ranges.valueOfFloat(System.getProperty("RANGE", MinMax.DEFAULT_RANGE));

        DirectProvider dp = new DirectProvider();
        Topology top = dp.newTopology();

        TStream<Float> tempReadings;

        if (Boolean.valueOf(System.getProperty("FIX_IO_CONTENT", MinMax.DEFAULT_FIX_IO_CONTENT))) {
            tempReadings = top.source(queryInput());
        } else {
            TempSensor sensor = new TempSensor();
            tempReadings = top.poll(sensor, 1, TimeUnit.MILLISECONDS);
        }

        TStream<Float> filteredReadings = tempReadings.filter(reading -> !values.contains(reading));

        filteredReadings.sink(reading -> {
            System.out.println(String.format("%.5f", reading));

            switch(Integer.valueOf(System.getProperty("QUERY_TYPE", MinMax.DEFAULT_QUERY_TYPE))) {
                case 0:
                    System.exit(0);
                    break;
                case 1:
                    break;
                default:
                    System.out.println("ERROR: Invalid query type.");
                    break;
            }
        });

        dp.submit(top);
    }
}