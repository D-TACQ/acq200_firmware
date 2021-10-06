/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: Job.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Job.java,v 1.4 2002/05/27 10:25:10 pgm Exp $
 * $Log: Job.java,v $
 * Revision 1.4  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import java.io.IOException;

public class Job implements Runnable
{
	public String getJobPath()
	{
		return jobPath;
	}

	public void setJobPath(String propValue)
	{
		String temp = jobPath;
		jobPath = propValue;
	}

	protected String jobPath = (String)new String("no-job");

    public void run () {
        if ( jobPath.compareTo( "no-job" ) == 0 ){
            return;
        }else{
            System.out.println( "Job.Run( "+jobPath+" )" );
            
            try {
                Runtime.getRuntime().exec( jobPath );
            } catch( IOException e ) {
                System.out.println( "ERROR:Job.Run( "+jobPath+" ) " + e );
            }
        }
    }
}