package dtacqutils;

import java.util.Calendar;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.text.NumberFormat;

/**
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: Jun 2, 2004
 * Time: 7:27:57 AM
 * To change this template use File | Settings | File Templates.
 */
public class TimedPathGenerator {

    long current_time_millis;
    Calendar calendar = Calendar.getInstance();
    NumberFormat nf = NumberFormat.getInstance();
    String base_fn;
    String root;

    protected void createPathName(){
            calendar.setTimeInMillis(current_time_millis);
            base_fn = root + "/" +
                    calendar.get(Calendar.DAY_OF_YEAR) + "/" +
                    nf.format(calendar.get(Calendar.HOUR_OF_DAY)) + "/" +
                    nf.format(calendar.get(Calendar.MINUTE)) + "/" +
                    nf.format(calendar.get(Calendar.SECOND)) + "/";

            try {
                File file = new File(base_fn);
                file.mkdirs();
            } catch (Exception e) {
                System.err.println("FAILED to create " + base_fn + " " + e);
            }
    }
    public TimedPathGenerator(String root) {
        this.root = root;
        nf.setMinimumIntegerDigits( 2 );
        nf.setGroupingUsed( false );
        current_time_millis = System.currentTimeMillis();
        createPathName();
    }

    public boolean onNewSecond() {
        long time_now = System.currentTimeMillis();

        if (time_now > current_time_millis && time_now - current_time_millis > 1000 ||
            time_now < current_time_millis && current_time_millis - time_now > 1000) {
            current_time_millis = time_now;
            createPathName();
            return true;
        }else{
            return false;
        }
    }

    public String getPathName() {
        return base_fn;
    }

    public static void main(String args[]) {
        File file = new File("/tmp", "TimedPathGenerator.test");
        file.mkdirs();

        TimedPathGenerator tpg = new TimedPathGenerator(file.getAbsolutePath());
        NumberFormat nf = NumberFormat.getInstance();
        nf.setMinimumIntegerDigits( 3 );
        nf.setGroupingUsed( false );


        for (int ii = 1; ii != 100; ++ii) {
            tpg.onNewSecond();
            FileOutputStream fos = null;
            try {
                fos = new FileOutputStream(tpg.getPathName() + "/" + nf.format(ii));
            } catch (FileNotFoundException e) {
                e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
            }
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
            }
            try {
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
            }
            System.err.println( ii );
        }
    }
}
