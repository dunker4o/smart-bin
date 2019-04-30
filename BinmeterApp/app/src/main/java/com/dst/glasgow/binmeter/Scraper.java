package com.dst.glasgow.binmeter;

import android.util.Log;

import org.jsoup.HttpStatusException;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;


public class Scraper {

	private String url;
	private Document document;
	private String distance;
	private String duration;
	private boolean isEmpty;
	
	public Scraper(String url) {
		super();
		this.url = url;
		this.isEmpty = true;
	}

	public Scraper() {
		super();
	}
	
	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}
	
	public Document getDocument() {
		return document;
	}

	public void setDocument(Document document) {
		this.document = document;
	}
	
	public void scrapeDocument() throws Exception{
		try{
			this.document = Jsoup.connect(url).get();
			isEmpty = false;
		}catch(HttpStatusException e){
			e.printStackTrace();
			isEmpty = true;
		}
	}

	public boolean isEmpty(){
		return this.isEmpty;
	}

	public String getDistance() {
		return distance;
	}
	
	public void setDistance(String distance) {
		this.distance = distance;
	}

	public void updateDistance(){
		this.distance = document.select("#distanceVal").text();
	}
	
	public String getDuration() {
		return duration;
	}

	public void setDuration(String duration) {
		this.duration = duration;
	}

	public void updateDuration(){
		this.duration = document.select("#durationVal").text();
	}
}
