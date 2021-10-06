/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: XYDataSetTableAdapter.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: XYDataSetTableAdapter.java,v 1.2.4.1 2004/09/03 08:42:26 pgm Exp $
 * $Log: XYDataSetTableAdapter.java,v $
 * Revision 1.2.4.1  2004/09/03 08:42:26  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.2  2002/11/09 19:47:11  pgm
 * jfreechart 0.9.4
 *
 * Revision 1.1  2002/05/27 10:27:23  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/

// XYDataSetTableAdapter.java

package xyplot;

import com.jrefinery.data.*;
import javax.swing.table.*;

import debug.Debug;

public class XYDataSetTableAdapter
extends AbstractSeriesDataset implements XYDataset {

   // use the translate to change the data and demonstrate dynamic data changes
    private TableModel model;

    /**
     * Default constructor.
     */
    public XYDataSetTableAdapter() {
	Debug.println( "XYDataSetTableAdapter()" );
    }

    public void setModel( TableModel model ) {
	this.model = model;
	onChange();
    }
    /**
     * Returns the x-value for the specified series and item.  Series are numbered 0, 1, ...
     * @param series The index (zero-based) of the series;
     * @param item The index (zero-based) of the required item;
     * @return The x-value for the specified series and item.
     */

    private boolean firstX = true;
    private boolean firstY = true;

    public Number getXValue(int series, int item) {
	//	if ( firstX ){
	//	    System.out.println( "getXValue() " + new Integer(item) );
	//  firstX = false;
	//}
        return new Integer( item);
    }
    public Number getYValue(int series, int item) {
	//	if ( firstY ){
	//	    System.out.println( "getYValue( "+series+", "+item+") " );
	//	    firstY = false;
	//	}
	// getValueAt( row, col ); !!
        return  (Number)model.getValueAt( item, series );
    }


    /**
     * Returns the number of series in the data source.
     * @return The number of series in the data source.
     */
    public int getSeriesCount() {
	// System.out.println( "getSeriesCount " + model.getColumnCount() );
        return model.getColumnCount();
    }

    /**
     * Returns the name of the series.
     * @param series The index (zero-based) of the series;
     * @return The name of the series.
     */
    public String getSeriesName(int series) {
	//	System.out.println( "getSeriesName " );
	//	System.out.println( "getSeriesName "+ model.getColumnName( series ) );
	return model.getColumnName( series );
    }

    /**
     * Returns the number of items in the specified series.
     * @param series The index (zero-based) of the series;
     * @return The number of items in the specified series.
     */
    public int getItemCount(int series) {
	//	System.out.println( "getItemCount " + model.getRowCount() );
        return model.getRowCount();
    }

    public void onChange() {
        this.notifyListeners(new DatasetChangeEvent(this, this));
    }
}
