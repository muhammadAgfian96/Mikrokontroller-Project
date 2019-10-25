package com.blogspot.mekatronika.lampproject;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.os.Bundle;
import android.widget.Toast;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.AxisBase;
import com.github.mikephil.charting.components.Description;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.LegendEntry;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.formatter.IValueFormatter;
import com.github.mikephil.charting.formatter.IndexAxisValueFormatter;
import com.github.mikephil.charting.formatter.ValueFormatter;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.ViewPortHandler;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;
import com.google.firebase.database.ValueEventListener;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

public class graph extends AppCompatActivity {

    //    inisialisasi grafik
    LineChart mChart;
    ArrayList<Entry> yData;
    long[] timeStamp =new long[3000];

    // inisialisasi firebase
    FirebaseDatabase database;
    DatabaseReference refCahaya;

    int i = 0;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_graph);

        mChart = (LineChart) findViewById(R.id.line_chart1);



        /*
        // Definisiin line pertama dan add data
        LineDataSet lineDataSet1 = new LineDataSet(dataValues1(), "Cahaya");
        // styling

        // add ad data
        ArrayList<ILineDataSet> dataSets = new ArrayList<>();
        dataSets.add(lineDataSet1);


        // mengintegrasikan seluruh data
        LineData data = new LineData(dataSets);
        mChart.setData(data); // memasukkan data ke mChart
        mChart.invalidate();
        */

        Query myCahaya;

        database = FirebaseDatabase.getInstance();
        refCahaya = database.getReference("Ruang_Ngoprek").child("paket_json");
        myCahaya =refCahaya.limitToLast(2000);

    myCahaya.addValueEventListener(new ValueEventListener() {
        @Override
        public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
            if (dataSnapshot != null){

                yData = new ArrayList<>();


                i=0;

                for (DataSnapshot ds : dataSnapshot.getChildren()){
                    String yLight = ds.child("Set_Data").getValue().toString();
                    String tStamp = ds.child("Timestamp").getValue().toString();

                    if (!tStamp.isEmpty()){

                        Float sValue = Float.parseFloat(yLight);
                        Long tStamp_int = Long.parseLong(tStamp);

                        timeStamp[i] = tStamp_int;
                        yData.add(new Entry(i, sValue));
                        i += 1;
                    }

                }



                final LineDataSet lineDataSet1 = new LineDataSet(yData, "Light");


                // ********* START STYLNG GRAPH **********************

                // region styling

                // # mengatur komponen umum graph
                mChart.setDragEnabled(true);
                mChart.setScaleEnabled(true);
                mChart.setBackgroundColor(Color.WHITE);

                mChart.setNoDataText("Loading Data");
                mChart.setNoDataTextColor(Color.parseColor("#00A2D8"));

                mChart.setDrawBorders(true);
                mChart.setBorderWidth(2);
                mChart.setBorderColor(Color.parseColor("#90AFAFAF"));


                // # menon aktifkan gridbackground
//        mChart.getXAxis().setDrawGridLines(false);
//        mChart.getAxisLeft().setDrawGridLines(false);


                // Mengatur styling garis data
                // ** garis
                lineDataSet1.setLineWidth(2);
                lineDataSet1.setColor(Color.parseColor("#00A2D8"));


                lineDataSet1.enableDashedHighlightLine(20, 10,0);
                lineDataSet1.setHighlightLineWidth(5);
                lineDataSet1.setHighlightLineWidth(Color.RED);
//        lineDataSet1.enableDashedLine(20,10,0);

                // ** point
                lineDataSet1.setDrawCircles(true);
                lineDataSet1.setDrawCircleHole(false);
                lineDataSet1.setCircleColor(Color.RED);
                lineDataSet1.setCircleRadius(5);

                // ** text on point
                lineDataSet1.setDrawValues(true);
                lineDataSet1.setValueTextSize(12);
                lineDataSet1.setValueTextColor(Color.GRAY);
//                lineDataSet1.setValueFormatter(new FormatterTextDataValues());

                // ** Setting Axis X
                XAxis xAxis = mChart.getXAxis();
                xAxis.setLabelRotationAngle(-45);
                xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
                xAxis.setTextSize(12);
                xAxis.setTextColor(Color.parseColor("#57C9E9"));
                xAxis.setYOffset(5);
                xAxis.setValueFormatter(new FormatterX_AxisValues());

                mChart.getAxisRight().setEnabled(false);

                YAxis yAxisLeft = mChart.getAxisLeft();
                yAxisLeft.setTextSize(12);
                yAxisLeft.setTextColor(Color.parseColor("#57C9E9"));


                // # Mengatur Legend, penamaan bergantung label pada LabelDataset1 di atas
                Legend legend = mChart.getLegend();
                legend.setEnabled(true);
                legend.setForm(Legend.LegendForm.LINE);
                legend.setTextSize(14);
                legend.setYOffset(5);
                legend.setTextColor(Color.parseColor("#00A2D8"));


                // # Mengatur Description graph
                Description desc = new Description();
                desc.setText("Cahaya di Kamar");
                desc.setTextColor(Color.parseColor("#00A2D8"));
                desc.setTextSize(14);
                desc.setYOffset(8);
                desc.setXOffset(4);
                mChart.setDescription(desc);
// endregion styling

                // ********** END STYLING GRAPH *********************

                // add data
                ArrayList<ILineDataSet> dataSets = new ArrayList<>();
                dataSets.add(lineDataSet1);

                LineData data = new LineData(dataSets);

                mChart.setData(data);
                mChart.notifyDataSetChanged();
                mChart.invalidate();

//                mChart.setVisibleXRangeMaximum(3600000);
                mChart.setVisibleXRangeMaximum(20);
                mChart.moveViewToX(data.getXMax());

            }else {
                Toast.makeText(getApplicationContext(), "Empty Data", Toast.LENGTH_SHORT).show();
            }


        }

        @Override
        public void onCancelled(@NonNull DatabaseError databaseError) {
            Toast.makeText(getApplicationContext(), "Failed Get Data", Toast.LENGTH_SHORT).show();

        }
    });







    }

    private ArrayList<Entry> dataValues1(){
        ArrayList<Entry> dataVals = new ArrayList<Entry>();
        dataVals.add(new Entry(0,20));
        dataVals.add(new Entry(1,30));
        dataVals.add(new Entry(2,40));
        dataVals.add(new Entry(3,10));
        dataVals.add(new Entry(4,25));

        return dataVals;
    }

    private class FormatterTextDataValues extends ValueFormatter {
        @Override
        public String getFormattedValue(float value) {
            return value +" mA";
        }
    }

    private class FormatterX_AxisValues extends ValueFormatter{
        @Override
        public String getAxisLabel(float value, AxisBase axis) {

            axis.setLabelCount(5, true);

            if(timeStamp != null){
                Date date = new Date((long) timeStamp[(int) Math.floor(value)]);
                SimpleDateFormat sdf = new SimpleDateFormat("dd HH:mm:ss", Locale.ENGLISH);
                return sdf.format(date);}
            else{
                return String.valueOf(value);
                }

        }
    }


    private long diffTime(Long startTime, Long endTime){
        Long diff = 0L;
        Date start = new Date(startTime);
        Date end = new Date(endTime);

        diff = start.getTime() - end.getTime();

        return diff;
    }

}

