package com.chilloutsoft.transportmania;

import org.libsdl.app.SDLActivity;

import android.os.Bundle;
//import android.preference.PreferenceManager;
import android.util.Log;
import android.content.Context;
//import android.content.SharedPreferences;
import android.content.res.AssetManager;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;
//import android.os.Environment;
import java.io.File;
import java.lang.Exception;

public class TransportManiaActivity extends SDLActivity {
	static {
		System.loadLibrary("TransportManiaNative");
	}

	//private static final String PREFERENCE_FIRST_RUN = "PREFERENCE_FIRST_RUN";
	private static boolean FORCE_RESET_ASSETS = false;
	private static final String ASSETS_LOCATION = "/TransportMania/";

	void createFolderIfNotExist(String path) throws Exception {
		File folder = new File(path);

		boolean success = true;
		if (!folder.exists()) {
			success = folder.mkdirs();
		}
		if (!success) {
			throw new Exception("Failed to create folder: " + path);
		}
	}

	private void copyFromAssets(String assetsFolder, String folderPath, String fileName) throws Exception {
		
		String pathOnPhone = folderPath + "/" + fileName;
		String pathInAssets = assetsFolder + "/" + fileName;

		createFolderIfNotExist(folderPath);

		AssetManager assetManager = getAssets();
		InputStream in = assetManager.open(pathInAssets);
		
		OutputStream out = new FileOutputStream(pathOnPhone);

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

	private void copyFilesFromAssets(String pathToPhoneAssets, String assetsFolder) throws Exception {

		String pathToFolderOnPhone = new String(pathToPhoneAssets + ASSETS_LOCATION + assetsFolder);
		
		AssetManager assetManager = getAssets();
		String [] assetsInFolder = assetManager.list(assetsFolder);

		for (String f : assetsInFolder)
		{
			if (!FORCE_RESET_ASSETS)
			{
				String pathToFileOnPhone = pathToFolderOnPhone + "/" + f;
				
				if (new File(pathToFileOnPhone).exists())
					continue;
			}
			
			copyFromAssets(assetsFolder, pathToFolderOnPhone, f);
		}
	}
	
	private void resetAssets(String pathToPhoneAssets) {
		Log.v("TRM", "Resetting assets...");
		
		try {
			Log.v("TRM", "Fonts...");
			copyFilesFromAssets(pathToPhoneAssets, "Fonts");
			Log.v("TRM", "Height Maps...");
			copyFilesFromAssets(pathToPhoneAssets, "HeightMaps");
			Log.v("TRM", "Models...");
			copyFilesFromAssets(pathToPhoneAssets, "Models");
			Log.v("TRM", "Textures...");
			copyFilesFromAssets(pathToPhoneAssets, "Textures");
		} catch (Exception e) {
			Log.e("TRM", "Failed to reset assets: " + e.toString());
		}

		Log.v("TRM", "Resetting finished");
	}

	// Setup
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		//Context context = getContext();
		//String pathToAssetsOnPhone = context.getExternalFilesDir(null).toString();
		String pathToAssetsOnPhone = "/sdcard";

		resetAssets(pathToAssetsOnPhone);
	}
}
