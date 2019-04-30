package com.dst.glasgow.binmeter;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.support.v4.app.NotificationCompat;
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

        if(prefs.getBoolean("display_capacity_percent", false) == false){
            TextView percentView = (TextView) findViewById(R.id.cap_percent);
            percentView.setVisibility(View.GONE);
        }else{
            TextView capacityView = (TextView) findViewById(R.id.bin_cap);
            capacityView.setVisibility(View.GONE);
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

                    if(!scrap.isEmpty()) {
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
                                TextView percentView = (TextView) findViewById(R.id.cap_percent);
                                if (currentDistance > 50) {
                                    capacityView.setText("Empty");
                                    percentView.setText("100%");
                                } else if (currentDistance > 34) {
                                    capacityView.setText("Barely anything in it");
                                    percentView.setText("75%");
                                } else if (currentDistance > 23) {
                                    capacityView.setText("Halfway full");
                                    percentView.setText("50%");
                                } else if (currentDistance > 11) {
                                    capacityView.setText("Almost full");
                                    percentView.setText("25%");
                                } else if (currentDistance > 5) {
                                    capacityView.setText("FULL");
                                    percentView.setText("0%");
                                    if(prefs.getBoolean("notifications_new_message", true)) {
                                        String binName = prefs.getString("bin_name", "Test01");
                                        String notificationMessage = binName + " has been filled. Time to clean it up.";
                                        sendNotification(findViewById(android.R.id.content), notificationMessage);
                                    }
                                } else {
                                    capacityView.setText("Overflowing");
                                    percentView.setText("-10%");
                                }

                            }
                        });
                    }
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

    public void sendNotification(View v, String notification){


    NotificationManager mNotificationManager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
    // The id of the channel.
    String id = "sBin_channel_01";

    // The user-visible name of the channel.
    CharSequence name = "Smart Bin";

    // The user-visible description of the channel.
    String description = "Smart Bin notifications";

    int importance = NotificationManager.IMPORTANCE_LOW;

    NotificationChannel mChannel = new NotificationChannel(id, name,importance);

    // Configure the notification channel.
    mChannel.setDescription(description);

    mChannel.enableLights(true);
    // Sets the notification light color for notifications posted to this
    // channel, if the device supports this feature.
    mChannel.setLightColor(Color.RED);

    mChannel.enableVibration(true);
    mChannel.setVibrationPattern(new long[]{100, 200, 300, 400, 500, 400, 300, 200, 400});

    mNotificationManager.createNotificationChannel(mChannel);


    NotificationManager mNotification = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
    PendingIntent notifyPIntent =
                PendingIntent.getActivity(getApplicationContext(), 0, new Intent(), 0);


    Notification toNotify = new Notification.Builder
                (getApplicationContext()).setContentTitle("Smart Bin").setContentText(notification).
                setSmallIcon(R.mipmap.ic_launcher_foreground).setChannelId(id).
            setContentIntent(notifyPIntent).build();



    toNotify.flags |= Notification.FLAG_AUTO_CANCEL;
    mNotification.notify(001, toNotify);

    }
}
