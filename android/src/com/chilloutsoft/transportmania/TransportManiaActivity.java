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
		
		Log.v("TRM", Environment.getExternalStorageDirectory().toString());
		
		String path = new String(Environment.getExternalStorageDirectory()
				.getPath() + ASSETS_LOCATION + fileName);
		
		Log.v("TRM", fileName);
		Log.v("TRM", path);
		
		AssetManager assetManager = getAssets();
		
		InputStream in = assetManager.open(fileName);

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
		
		Log.v("TRM", "File " + fileName + " copied");
	}

	private void copyFilesFromFolder(String folder) throws Exception {
		String pathToFolder = new String(Environment.getExternalStorageDirectory()
			.getPath() + ASSETS_LOCATION + folder);

		File filesFolder = new File(pathToFolder);
		File [] files = filesFolder.listFiles();
		
		for (File f : files) {
			
			if (f.exists() || !FORCE_RESET_ASSETS)
				continue;
			
		    copyFile(folder + '/' + f.getName());
		}
	}
	
	private void copyFonts() throws Exception {
		Log.v("TRM", "Fonts...");
		copyFilesFromFolder("Fonts");
	}

	private void copyHeightMaps() throws Exception {
		Log.v("TRM", "Height Maps...");
		copyFilesFromFolder("HeightMaps");
	}

	private void copyModels() throws Exception {
		Log.v("TRM", "Models...");
		copyFilesFromFolder("Models");
	}

	private void copyTextures() throws Exception {
		Log.v("TRM", "Textures...");
		copyFilesFromFolder("Textures");
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
		//SharedPreferences p = PreferenceManager
		//		.getDefaultSharedPreferences(this);
		//boolean firstRun = p.getBoolean(PREFERENCE_FIRST_RUN, true);

		//if (FORCE_RESET_ASSETS || firstRun) {
			resetAssets();
			//p.edit().putBoolean(PREFERENCE_FIRST_RUN, false).commit();
		//}
	}
}
