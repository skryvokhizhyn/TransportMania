package com.chilloutsoft.transportmania;

import org.libsdl.app.SDLActivity;

import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;
import android.os.Environment;
import java.io.File;
import java.lang.Exception;

public class TransportManiaActivity extends SDLActivity {
	static {
		System.loadLibrary("TransportManiaNative");
	}

	private static final String PREFERENCE_FIRST_RUN = "PREFERENCE_FIRST_RUN";
	private static boolean FORCE_RESET_ASSETS = false;
	private static final String ASSETS_LOCATION = "/TransportMania/";

	void createParentFolderIfNotExist(String path) throws Exception {
		File parentFolder = new File(path).getParentFile();

		boolean success = true;
		if (!parentFolder.exists()) {
			success = parentFolder.mkdirs();
		}
		if (!success) {
			throw new Exception("Failed to create folder: " + path);
		}
	}

	private void copyFile(String fileName) throws Exception {
		AssetManager assetManager = getAssets();

		InputStream in = assetManager.open(fileName);
		String path = new String(Environment.getExternalStorageDirectory()
				.getPath() + ASSETS_LOCATION + fileName);

		createParentFolderIfNotExist(path);

		OutputStream out = new FileOutputStream(path);

		byte[] buffer = new byte[1024];

		int read;
		while ((read = in.read(buffer)) != -1) {
			out.write(buffer, 0, read);
		}

		in.close();
		in = null;

		out.flush();
		out.close();
		out = null;
	}

	private void copyFonts() throws Exception {
		copyFile("Fonts/arial_ttf_cyr_lat.fnt");
	}

	private void copyHeightMaps() throws Exception {
		copyFile("HeightMaps/hf_513.bmp");
	}

	private void copyModels() throws Exception {
		copyFile("Models/wagon.obj");
	}

	private void copyTextures() throws Exception {
		copyFile("Textures/arial_ttf_cyr_lat_0.png");
		copyFile("Textures/grass.png");
		copyFile("Textures/graybackground.png");
		copyFile("Textures/okbutton.png");
		copyFile("Textures/railway.png");
		copyFile("Textures/windowbox.png");
		copyFile("Textures/woodwagon.png");
	}

	private void resetAssets() {
		Log.v("TRM", "Resetting assets...");

		try {
			copyFonts();
			copyHeightMaps();
			copyModels();
			copyTextures();
		} catch (Exception e) {
			Log.e("TRM", "Failed to reset assets: " + e.toString());
		}

		Log.v("TRM", "Resetting finished");
	}

	// Setup
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// do first run assets deployment
		SharedPreferences p = PreferenceManager
				.getDefaultSharedPreferences(this);
		boolean firstRun = p.getBoolean(PREFERENCE_FIRST_RUN, true);

		if (FORCE_RESET_ASSETS || firstRun) {
			p.edit().putBoolean(PREFERENCE_FIRST_RUN, false).commit();
			resetAssets();
		}
	}
}
