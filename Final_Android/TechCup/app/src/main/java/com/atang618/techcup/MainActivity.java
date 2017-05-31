package com.atang618.techcup;

// libraries

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

import java.io.IOException;

import static android.graphics.Color.blue;
import static android.graphics.Color.green;
import static android.graphics.Color.red;
import static android.graphics.Color.rgb;

public class MainActivity extends Activity implements TextureView.SurfaceTextureListener {
    private Camera mCamera;
    private TextureView mTextureView;
    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;
    private Bitmap bmp = Bitmap.createBitmap(640, 480, Bitmap.Config.ARGB_8888);
    private Canvas canvas = new Canvas(bmp);
    private Paint paint1 = new Paint();
    private TextView mTextView1;
    private TextView mTextView2;
    private TextView mTextView4;
    private SeekBar myControl;
    private SeekBar yStart;

    static long prevtime = 0; // for FPS calculation
    static char direction = 'C';
    static int location;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); // keeps the screen from turning off

        mTextView2 = (TextView) findViewById(R.id.cameraStatus);
        myControl = (SeekBar) findViewById(R.id.seek1);

        yStart = (SeekBar) findViewById(R.id.seek3);
        mTextView1 = (TextView) findViewById(R.id.textView01);
        mTextView1.setText("Thresh = 0");

        mTextView4 = (TextView) findViewById(R.id.textView04);
        mTextView4.setText("yStart = 0");
        setMyControlListener();

        setYStartListener();

        // see if the app has permission to use the camera
        ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.CAMERA}, 1);
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED) {
            mSurfaceView = (SurfaceView) findViewById(R.id.surfaceview);
            mSurfaceHolder = mSurfaceView.getHolder();

            mTextureView = (TextureView) findViewById(R.id.textureview);
            mTextureView.setSurfaceTextureListener(this);

            // set the paintbrush for writing text on the image
            paint1.setColor(0xffff0000); // red
            paint1.setTextSize(24);

            mTextView2.setText("started camera");
        } else {
            mTextView2.setText("no camera permissions");
        }

    }

    private void setMyControlListener() {
        myControl.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

            int progressChanged = 0;

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progressChanged = progress;
                mTextView1.setText("Thresh = " + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

    }


    private void setYStartListener() {
        yStart.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                mTextView4.setText("yStart = " + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

    }

    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        mCamera = Camera.open();
        Camera.Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(640, 480);
        parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_INFINITY); // no autofocusing
        parameters.setAutoExposureLock(true); // keep the white balance constant
        mCamera.setParameters(parameters);
        mCamera.setDisplayOrientation(90); // rotate to portrait mode

        try {
            mCamera.setPreviewTexture(surface);
            mCamera.startPreview();
        } catch (IOException ioe) {
            // Something bad happened
        }
    }

    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        // Ignored, Camera does all the work for us
    }

    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        mCamera.stopPreview();
        mCamera.release();
        return true;
    }

    // the important function
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        // every time there is a new Camera preview frame
        mTextureView.getBitmap(bmp);

        final Canvas c = mSurfaceHolder.lockCanvas();
        int startY = yStart.getProgress()*5;
        int endY = startY + 10;
        if (c != null) {
            int thresh = myControl.getProgress();
            int[] pixels = new int[bmp.getWidth()]; // pixels[] is the RGBA data
             // which row in the bitmap to analyze to read
            int totalMass = 0;
            int massIndex = 0;
            for (int j = startY; j < endY; j += 5) {
                bmp.getPixels(pixels, 0, bmp.getWidth(), 0, j, bmp.getWidth(), 1);

                // in the row, see if there is more green than red
                for (int i = 0; i < bmp.getWidth(); i++) {
                    if ((green(pixels[i]) - red(pixels[i])) > thresh) {
                        pixels[i] = rgb(0, 255, 0); // over write the pixel with pure green

                    } else if ((blue(pixels[i]) - red(pixels[i])) > thresh) {
                        pixels[i] = rgb(0, 0, 255);
                    } else {
                        massIndex += i;
                        totalMass += 1;
                    }
                }
                // update the row
                bmp.setPixels(pixels, 0, bmp.getWidth(), 0, j, bmp.getWidth(), 1);
            }
            location = massIndex / totalMass;
        }

        // draw a circle at some position

        canvas.drawCircle(location, startY, 4, paint1); // x position, y position, diameter, color
        if (location > 245) {
            direction = 'L';
        } else if (location < 245) {
            direction = 'R';
        } else {
            direction = 'C';
        }
        // canvas.drawRect(boundary_left,yStart.getProgress(),boundary_left,yStart.getProgress() + 200, paint1);
        // write the pos as text
        canvas.drawText("Direction = " + direction, 20, 420, paint1);
        c.drawBitmap(bmp, 0, 0, null);
        mSurfaceHolder.unlockCanvasAndPost(c);


        // calculate the FPS to see how fast the code is running
        long nowtime = System.currentTimeMillis();
        long diff = nowtime - prevtime;
        mTextView2.setText("FPS " + 1000 / diff);
        prevtime = nowtime;
    }
}