package com.example.imagecompressor;

import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.text.DecimalFormat;

public class MainActivity extends AppCompatActivity {

    ImageView imageView,imageView1;
    Button button,button1;
    Bitmap bmp;

    InputStream is = null;
    private static final int PICK_IMAGE = 100;
    Uri imageUri;
    TextView tv1,tv;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("combine");
        //System.loadLibrary("opencv_java3");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv = findViewById(R.id.sample_text);
        tv1= findViewById(R.id.sample_text2);


            imageView = (ImageView)findViewById(R.id.preview);
            imageView1 = (ImageView)findViewById(R.id.result);
            button = (Button)findViewById(R.id.Select);
            button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    openGallery();
                }
            });
        button1 = (Button)findViewById(R.id.Compress);
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                imageView1.setImageBitmap(bmp);

                tv1.setText( (bmp.getByteCount() / (1024 ))+ " kb");
            }
        });


    }

    private void openGallery() {
            Intent gallery = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.INTERNAL_CONTENT_URI);
            startActivityForResult(gallery, PICK_IMAGE);
        }
        @Override
        protected void onActivityResult(int requestCode, int resultCode, Intent data){
            super.onActivityResult(requestCode, resultCode, data);
            if (resultCode == RESULT_OK && requestCode == PICK_IMAGE){
                imageUri = data.getData();
                Bitmap bitmap = null;
                try {
                    bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(),imageUri);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                imageView.setImageBitmap(bitmap);
                double a = (bitmap.getByteCount() / (1024) );
                DecimalFormat dec= new DecimalFormat("#0.0");
                dec.format(a);
                //String.format("% .1f ",a);
                //tv.setText( a+" kb");
                //tv.setText((bitmap.getByteCount() /(1024*1024) )+ " Mb");
                //Mat tmp = new Mat(bitmap.getWidth(), bitmap.getHeight(), CvType.CV_8UC1);
                //Utils.bitmapToMat(bitmap, tmp);
                //ImageFromJNI(tmp.getNativeObjAddr());
                //sImageFromJNI(tmp.getNativeObjAddr());
                Mat tmp = new Mat(bitmap.getWidth(), bitmap.getHeight(), CvType.CV_8UC1);
                Utils.bitmapToMat(bitmap, tmp);
                Mat out=new Mat();

                 String folder="new_folder";
                AssetManager assetManager=getAssets();
                try {
                    is = assetManager.open("my_pic2.jpg");
                } catch (IOException e) {
                    e.printStackTrace();
                }
                 File f=new File(Environment.getExternalStorageDirectory(),folder);
                 if(!f.exists()){
                     f.mkdir();
                 }
                 String path = Environment.getExternalStorageDirectory().getPath().toString()+"/"+folder+"/";
                 tv.setText(path);
                ImageFromJNI(tmp.getNativeObjAddr(),out.getNativeObjAddr(),path);
                bmp = Bitmap.createBitmap(out.cols(),out.rows(), Bitmap.Config.ARGB_8888);

                Utils.matToBitmap(out,bmp);


            }
        }






    public native String ImageFromJNI(long nativeObjAddr, long objAddr, String path);
}

