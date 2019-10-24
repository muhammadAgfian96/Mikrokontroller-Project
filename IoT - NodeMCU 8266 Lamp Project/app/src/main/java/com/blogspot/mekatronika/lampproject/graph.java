package com.blogspot.mekatronika.lampproject;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.os.Bundle;

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

import java.util.ArrayList;

public class graph extends AppCompatActivity {

    //    inisialisasi grafik
    LineChart mChart;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_graph);

        mChart = (LineChart) findViewById(R.id.line_chart1);

        // Data pertama
        LineDataSet lineDataSet1 = new LineDataSet(dataValues1(), "Cahaya");
        ArrayList<ILineDataSet> dataSets = new ArrayList<>();
        dataSets.add(lineDataSet1);


        // mengatur komponen umum graph
        mChart.setDragEnabled(true);
        mChart.setScaleEnabled(true);
        mChart.setBackgroundColor(Color.WHITE);

        mChart.setNoDataText("Loading Data");
        mChart.setNoDataTextColor(Color.parseColor("#00A2D8"));

        mChart.setDrawBorders(true);
        mChart.setBorderWidth(2);
        mChart.setBorderColor(Color.parseColor("#90AFAFAF"));


        // menon aktifkan gridbackground
//        mChart.getXAxis().setDrawGridLines(false);
//        mChart.getAxisLeft().setDrawGridLines(false);


         // Mengatur styling garis data
        // # garis
        lineDataSet1.setLineWidth(2);
        lineDataSet1.setColor(Color.parseColor("#00A2D8"));


        lineDataSet1.enableDashedHighlightLine(20, 10,0);
        lineDataSet1.setHighlightLineWidth(5);
        lineDataSet1.setHighlightLineWidth(Color.RED);
//        lineDataSet1.enableDashedLine(20,10,0);

        // # point
        lineDataSet1.setDrawCircles(true);
        lineDataSet1.setDrawCircleHole(false);
        lineDataSet1.setCircleColor(Color.RED);
        lineDataSet1.setCircleRadius(5);

        // # text on point
        lineDataSet1.setDrawValues(true);
        lineDataSet1.setValueTextSize(12);
        lineDataSet1.setValueTextColor(Color.GRAY);
        lineDataSet1.setValueFormatter(new FormatterTextDataValues());

        // # Setting Axis
        XAxis xAxis = mChart.getXAxis();
        xAxis.setLabelRotationAngle(-90);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setTextSize(12);
        xAxis.setTextColor(Color.parseColor("#57C9E9"));
        xAxis.setYOffset(5);
        xAxis.setValueFormatter(new FormatterX_AxisValues());

        mChart.getAxisRight().setEnabled(false);

        YAxis yAxisLeft = mChart.getAxisLeft();
        yAxisLeft.setTextSize(12);
        yAxisLeft.setTextColor(Color.parseColor("#57C9E9"));


        // Mengatur Legend, penamaan bergantung label pada LabelDataset1 di atas
        Legend legend = mChart.getLegend();
        legend.setEnabled(true);
        legend.setForm(Legend.LegendForm.LINE);
        legend.setTextSize(14);
        legend.setYOffset(5);
        legend.setTextColor(Color.parseColor("#00A2D8"));


        // Mengatur Description graph
        Description desc = new Description();
        desc.setText("Cahaya di Kamar");
        desc.setTextColor(Color.parseColor("#00A2D8"));
        desc.setTextSize(14);
        desc.setYOffset(8);
        desc.setXOffset(4);
        mChart.setDescription(desc);


        // mengintegrasikan seluruh data
        LineData data = new LineData(dataSets);
        mChart.setData(data); // memasukkan data ke mChart
        mChart.invalidate();


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
            return value + " pm";
        }
    }
}

