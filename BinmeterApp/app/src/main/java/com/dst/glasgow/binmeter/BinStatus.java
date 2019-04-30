package com.dst.glasgow.binmeter;

import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.preference.PreferenceManager;

import org.w3c.dom.Text;

public class BinStatus extends AppCompatActivity {

    public String binURL;
    private SharedPreferences prefs;
    private double currentDistance;

    public void setIP(String url){
        binURL = "http://" + url;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bin_status);

        prefs = PreferenceManager.getDefaultSharedPreferences(this);
        setIP(prefs.getString("bin_url", "192.168.4.1"));

        if(prefs.getBoolean("display_debug_info", false) == false){
            LinearLayout durationLayout = (LinearLayout) findViewById(R.id.duration_layout);
            durationLayout.setVisibility(View.GONE);

            LinearLayout distanceLayout = (LinearLayout) findViewById(R.id.distance_layout);
            distanceLayout.setVisibility(View.GONE);
        }

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
                            currentDistance = Double.parseDouble(scrap.getDistance());

                            TextView durationView = (TextView) findViewById(R.id.duration_val);
                            durationView.setText(scrap.getDuration());

                            TextView distanceView = (TextView) findViewById(R.id.distance_val);
                            distanceView.setText(scrap.getDistance());

                            TextView capacityView = (TextView) findViewById(R.id.bin_cap);
                            if(currentDistance > 50){
                                capacityView.setText("Empty.");
                            }else if(currentDistance > 34){
                                capacityView.setText("Barely anything in it.");
                            }else if(currentDistance > 23){
                                capacityView.setText("Halfway full.");
                            }else if(currentDistance > 11){
                                capacityView.setText("Almost full.");
                            } else if (currentDistance > 5) {
                                capacityView.setText("FULL!");
                            } else{
                                capacityView.setText("Overflowing.");
                            }

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
