package com.blogspot.mekatronika.lampproject;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;
import com.google.firebase.database.ValueEventListener;


public class MainActivity extends AppCompatActivity {

//    inisialisasi button
    TextView btn_graph, btn_on_text, btn_off_text;
    TextView txt_Cahaya;
    LinearLayout btn_on, btn_off;

//    inisialisasi firebase
    FirebaseDatabase database;
    DatabaseReference refLamp, refCahaya;
    Query myCahaya;


    // inilisiasi
    boolean kondisi;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btn_graph = findViewById(R.id.btn_graph);
        btn_on = findViewById(R.id.btn_on);
        btn_off = findViewById(R.id.btn_off);
        btn_on_text = findViewById(R.id.btn_on_text);
        btn_off_text = findViewById(R.id.btn_off_text);
        txt_Cahaya = findViewById(R.id.txt_Cahaya);

        //
        database = FirebaseDatabase.getInstance();
        refCahaya = database.getReference("Ruang_Ngoprek").child("paket_json"); // path dari firebase
        myCahaya =refCahaya.limitToLast(5); // membatasi mengambil data (2000 dari terakhir)


        database = FirebaseDatabase.getInstance();
        refLamp = database.getReference("Ruang_Ngoprek").child("kondisi");




//        Cek Kondisi Lamp
        refLamp.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                String getLamp = dataSnapshot.getValue().toString();
//                Toast.makeText(getApplicationContext(), "Masuk listener = "+getLamp, Toast.LENGTH_SHORT).show();

                if (getLamp.equals("ON")){
                    Toast.makeText(getApplicationContext(), "Lampu Nyala", Toast.LENGTH_SHORT).show();

                    btn_on.setEnabled(false);
                    btn_on.setElevation(7);
                    btn_on_text.setTextColor(Color.parseColor("#7C878787")); // green

                    btn_off.setEnabled(true);
                    btn_off.setElevation(15);
                    btn_off_text.setTextColor(Color.parseColor("#00A2D8")); // gray

                }else if(getLamp.equals("OFF")){
                    btn_on.setEnabled(true);
                    btn_on.setElevation(15);
                    btn_on_text.setTextColor(Color.parseColor("#00A2D8")); //gray


                    btn_off.setEnabled(false);
                    btn_off.setElevation(7);
                    btn_off_text.setTextColor(Color.parseColor("#7C878787")); // merah


                    Toast.makeText(getApplicationContext(), "Lampu Mati", Toast.LENGTH_SHORT).show();

                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Toast.makeText(getApplicationContext(), "Failed", Toast.LENGTH_SHORT).show();
            }
        });


        txt_Cahaya.setTextColor(Color.RED);

        //      Cek the Last Data
        myCahaya.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                txt_Cahaya.setTextColor(Color.WHITE);
                for (DataSnapshot ds : dataSnapshot.getChildren()){
                    String cahaya = ds.child("Set_Data").getValue().toString();

                    if(!cahaya.isEmpty()){
                        txt_Cahaya.setText(cahaya);
                    }
                }

            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                txt_Cahaya.setTextColor(Color.RED);

            }
        });

        //    BUTTON ON DI PENCET
        btn_on.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                refLamp.setValue("ON");
            }
        });

        //    BUTTON OFF DI PENCET
        btn_off.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                refLamp.setValue("OFF");
            }
        });


        //    BUTTON GRAPH DI PENCET
        btn_graph.setOnClickListener(new View.OnClickListener() {
        @Override
            public void onClick(View view) {
                Intent goGraph = new Intent(MainActivity.this, graph.class);
                startActivity(goGraph);
            }
        });
    }


}
