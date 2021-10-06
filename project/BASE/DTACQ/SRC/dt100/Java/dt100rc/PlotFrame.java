/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: PlotFrame.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: PlotFrame.java,v 1.7.4.3 2004/09/03 08:42:26 pgm Exp $
 * $Log: PlotFrame.java,v $
 * Revision 1.7.4.3  2004/09/03 08:42:26  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.7.4.2  2004/06/22 20:46:18  pgm
 * only run StreamCallbacks on Frame Accept.
 *
 * Revision 1.7.4.1  2004/06/01 14:35:04  pgm
 * abandoned dt100gui package expt
 *
 * Revision 1.7  2002/11/09 21:27:50  pgm
 * fs range plot opt
 *
 * Revision 1.6  2002/11/09 19:47:11  pgm
 * jfreechart 0.9.4
 *
 * Revision 1.5  2002/05/26 11:52:10  pgm
 * good stream frame hiding behaviour
 *
 * Revision 1.4  2002/05/25 19:44:20  pgm
 * Dtacq LAF - bootiful
 *
 * Revision 1.3  2002/04/17 16:26:28  pgm
 * one adapter, many models
 *
 * Revision 1.2  2002/04/17 16:12:15  pgm
 * PlotFrame is The Frame
 *
 * Revision 1.1  2002/04/17 15:20:48  pgm
 * plotting from rc works ship it quick
 *
 * Revision 1.1.1.1  2000/06/03 05:36:25  pgm
 * fromQ
 *
 * Revision 1.1.1.1  2000/05/28 16:06:35  pgm
 * uses packages
 *
 * Revision 1.3  2000/04/15 17:42:01  pgm
 * sliders work from form designer
 *
 * Revision 1.2  2000/04/13 19:50:37  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
 \*****************************************************************************/



/*
A basic implementation of the JFrame class.
*/

import java.awt.*;
import javax.swing.*;
import javax.swing.table.*;

import com.jrefinery.data.*;
import com.jrefinery.data.*;
import com.jrefinery.chart.*;
import com.jrefinery.chart.data.*;
import com.jrefinery.chart.ui.*;
import com.jrefinery.layout.LCBLayout;
import com.jrefinery.ui.about.AboutFrame;
import xyplot.*;
import debug.Debug;


public class PlotFrame extends FrameWithModel {
    private String title = "title";
    private String domain = "sample";
    private String range = "amplitude";
    private JFreeChart chart;
    private XYDataSetTableAdapter data = new XYDataSetTableAdapter();
    private boolean scroll_pane = false;
    private boolean plot_full_scale;
    private double range_min;
    private double range_max;

    /**
     * The chart panel.
     */
    protected ChartPanel chartPanel;

    public void setModel(TableModel model) {

        try {
            data.setModel(model);

            if (chart == null) {
                configure();
            }
            if (plot_full_scale) {
                chart.getXYPlot().getRangeAxis().setRange(range_min, range_max);
            }
        } catch (java.lang.NullPointerException e) {
            System.err.println("PlotFrame NULL POINTER Exception - lost the plot " + e);
        }
    }

    public void setFullScale(boolean not_auto, double rmin, double rmax) {
        plot_full_scale = not_auto;
        range_min = rmin;
        range_max = rmax;
    }

    protected void configure() {
        try {
            //	    chart = ChartFactory.createXYChart(
            //			 title, domain, range, data, true);
            chart = ChartFactory.createXYChart("", domain, range, data, true);
        } catch (Exception e) {
            System.out.println("ChartFactory threw " + e);
            e.printStackTrace(System.err);
        }


        // then customise it a little...
        chart.setBackgroundPaint(new GradientPaint(0, 0, Color.white, 0, 1000, Color.green));

        chartPanel = new ChartPanel(chart);


        if (scroll_pane) {
            this.setContentPane(new JScrollPane(chartPanel));
        } else {
            this.setContentPane(chartPanel);
        }
        pack();
    }

    public void show() {

        super.show();
    }

    public PlotFrame() {
        this("D-Tacq default plot frame - using JFreeChart");
    }

    public PlotFrame(String sTitle) {
        super.setTitle(sTitle);
        Debug.println("PlotFrame()");
    }

    public void setPlotLabels(String title, String xaxis, String yaxis) {
        this.title = title;
        this.domain = xaxis;
        this.range = yaxis;
    }

    public ChartPanel getChartPanel() {
        return this.chartPanel;
    }

    public static void main(String[] args) {
        PlotFrame plotFrame = new PlotFrame("plot frame testharness");

        System.out.println("args len" + args.length);

        if (args.length == 3) {
            System.out.println("changing labels");

            plotFrame.setPlotLabels(args[0].compareTo("0") == 0 ?
                    args[0] : "",
                    args[1], args[2]);
        }
        plotFrame.setModel(new TestTableModel());


        plotFrame.setVisible(true);
    }
}
