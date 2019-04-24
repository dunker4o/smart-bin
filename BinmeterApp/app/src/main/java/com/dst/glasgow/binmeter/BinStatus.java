package com.dst.glasgow.binmeter;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import org.w3c.dom.Text;

public class BinStatus extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bin_status);

        Thread scrapeThread = new Thread(new Runnable() {
            @Override
            public void run() {
                final Scraper scrap = new Scraper("http://192.168.0.4");

                while(true){
                    try {
                        scrap.scrapeDocument();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    scrap.updateDistance();
                    scrap.updateDuration();

                    //This bit updates the fields. It has to be ran on the UI thread
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            TextView durationView = (TextView) findViewById(R.id.duration_val);
                            durationView.setText(scrap.getDuration());

                            TextView distanceView = (TextView) findViewById(R.id.distance_val);
                            distanceView.setText(scrap.getDistance());
                        }
                    });

                    try {
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }


            }
        });
        scrapeThread.start();


    }
}
