import java.util.Random;
import org.apache.edgent.function.Supplier;

/**
 * Every time get() is called, TempSensor generates a temperature reading.
 */
public class TempSensor implements Supplier<Float> {
    float currentTemp = 0.0f;
    Random rand;

    TempSensor(){
        rand = new Random();
    }

    @Override
    public Float get() {
        // Change the current temperature some random amount
        float newTemp = ((float) rand.nextGaussian()) + currentTemp;
        
        currentTemp = newTemp;
        return currentTemp;
    }
}