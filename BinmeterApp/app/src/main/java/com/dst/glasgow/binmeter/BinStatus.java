package com.dst.glasgow.binmeter;

import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.preference.PreferenceManager;

import org.w3c.dom.Text;

public class BinStatus extends AppCompatActivity {

    public String binURL;
    private SharedPreferences prefs;

    public void setIP(String url){
        binURL = "http://" + url;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bin_status);

        prefs = PreferenceManager.getDefaultSharedPreferences(this);
        setIP(prefs.getString("bin_url", "192.168.4.1"));

        Thread scrapeThread = new Thread(new Runnable() {
            @Override
            public void run() {
                final Scraper scrap = new Scraper(binURL);

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
