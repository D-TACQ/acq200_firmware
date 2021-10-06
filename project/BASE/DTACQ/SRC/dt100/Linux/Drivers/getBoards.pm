# getBoards.pm - builds acq32 device nodes from proc info

package getBoards;
require Exporter;

@ISA = qw( Exporter );
@EXPORT = qw(getBoards @DEVRECS getHeartbeat);



$GETBOARDS_REVID = '$Revision: 1.9.4.8 $';

print "getBoards $GETBOARDS_REVID - signing on includes ancient board hack\n";


%MAJORS;
%WAVMAJORS;
%ACQ200MAJORS;
$ndevices = 0;


sub getMajors {
    my ( $devices, $model ) = @_;

    my $rpre = '\s*([0-9]+)\s';
    my $rpst = '.([0-9]+)';

    my $rmodel = $rpre.$model.$rpst;
    my $rwav232= $rpre.'wav232out'.$rpst;
    my $racq200= $rpre.'acq200-bridge'.$rpst;

    open DEVICES, $devices or die "ERROR failed to open $devices";
    
    while ( my $line = <DEVICES> ){
	my $major, $ix;

	if ( (( $major, $ix ) = $line =~ /$rmodel/ ) ){
            $MAJORS{$ix} = $major;	        
	    ++$ndevices;
	}elsif ( (( $major, $ix ) = $line =~ /$rwav232/ )){
	    $WAVMAJORS{$ix} = $major;
	}elsif ( (( $major, $ix ) = $line =~ /$racq200/ )    ){
	    $ACQ200MAJORS{$ix} = $major;
	}
    }
}


@DEVRECS = ();

sub processBusLine {
    my ( $refb, $line ) = @_;

# bus file looks like this:
# bus fun irq        IO       SRAM        ROM
#0  0 32 0x05 0xd9400000 0xd9000000 0x00000000

    my $fields_regex = '(\w+)\s+' x6 . '(\w+)';    

    (
	  $refb->{devnum},
	  $refb->{bus},
	  $refb->{fun},
	  $refb->{irq},
	  $refb->{IO},
	  $refb->{SRAM},
          $refb->{ROM}     ) = $line =~ /$fields_regex/;
}

# Data file looks like this
#
#model chn      s/n       arm_fw       lca_fw     cal date cycle heartbeat
#ACQ32  32   D20026     boot66c3    acq32-lca       020323     0  93

# but if the firmware is really ancient it may look like:
#xxxxx 0 000623   d20014       boot66          lca                  0   0

# and semi-ancient like this
#ACQ32 0 010914   d21004       boot66    acq32-lca                 15   0
sub parseBoardRec {
    my $wf = '([\w-]+)\s+';
    my $nf = '([0-9]+)\s+';
    my $nf2 = $nf x 2;
    my $wf3 = $wf x 3;
    my $wf6 = $wf x 6;

    my $fields_regex = $wf.$nf.$wf6.'(\w+)';
    my $ancient_fields_regex = 'xxxxx ([0-9]+)\s+'.'([\w-]+)\s+' x 2;
    my $medieval_regex = $wf.$nf2.$wf3.$nf2;
    my $blank_regex = '^(BLANK)\s+([0-9])+';

    my ( $line ) = @_;
    my %brec;

    if ( ( 
       $brec{model},
       $brec{devnum} ) = $line =~ /$blank_regex/ ){
	; # goto OK
    }elsif ( ( 
      $brec{model}, 
      $brec{devnum},
      $brec{num_channels}, 
      $brec{serial_num}, 
      $brec{arm_fw}, 
      $brec{lca_fw}, 
      $brec{cal_date}, 
      $brec{cycle}, 
      $brec{heartbeat} ) = $line =~ /$fields_regex/ ){
        ;	# goto OK
    }elsif( (
	$brec{devnum},
	$brec{cal_date},
	$brec{serial_num},
				) = $line =~ /$ancient_fields_regex/ ){

	$brec{model} 		= 'ACQ32';
	$brec{num_channels} 	= '32';
	$brec{arm_fw}		= 'boot66c3';
	$brec{lca_fw}		= 'acq32-lca';
	$brec{cycle}		= '0';
	$brec{heartbeat}	= '0';
    }elsif ( (
	       $brec{model},
	       $brec{devnum},
	       $brec{cal_date},
	       $brec{serial_num},
	       $brec{arm_fw},
	       $brec{lca_fw},
	       $brec{cycle},
	       $brec{heartbeat} ) = $line =~ /$medieval_regex/ ){

	if ( $brec{model} =~ /ACQ32/ ){
	    $brec{num_channels} = 32;
	    $brec{arm_fw} = 'boot66c3';
	}elsif ( $brec{model} =~ /ACQ16/ ){
	    $brec{num_channels} = 16;
	    $brec{arm_fw} = 'boot66c2';
	}else{
	    return 0;
	}
    }else{
	return 0;
    }

# here with a valid %brec, complete it and return
    $brec{MAJOR} = $MAJORS{$brec{devnum}};

    $brec{WAVMAJOR} = $WAVMAJORS{$brec{devnum}};
    $brec{ACQ200MAJOR} = $ACQ200MAJORS{$brec{devnum}};
    if ( $brec{ACQ200MAJOR} ){
	$brec{DATA_MAJOR} = $brec{ACQ200MAJOR};
    }else{
	$brec{DATA_MAJOR} = $brec{MAJOR};
    }



    return \%brec
}


sub getResourceBoardNum {
    my ($refbrec) = @_;

    if ( $refbrec != 0 ){
	$refbrec->{lognum} = $refbrec->{devnum}+1;
    }
    return $refbrec;
}

sub getSlotBoardNum {
    my ($refbrec ) = @_;

    if ( $refbrec != 0 ){

	if ( $refbrec->{fun} >= 72 && $refbrec->{fun} <= 120 ){
;# works for CPV5350 in Schroff backplane
	    $refbrec->{lognum} = 9-($refbrec->{fun}/8-8);
#	    print "getSlotBoardNum() returns $refbrec->{lognum}\n";
	}else{
	    die "Slot addressing scheme failed";
	}
    }

    return $refbrec;
}


sub getBoards {
    my ( $procroot, $slotbased, $model, $test ) = @_;

#    print "getBoards procroot=$procroot\n";

    my $devsfile = $procroot . '/devices';
    my $procfile = $procroot . '/' . $model;

    getMajors( $devsfile, $model );
    printf "ndevices $ndevices\n";

    for ( $ix = 0; $ix != $ndevices; ++$ix ){
	;
#	print "Device:acq32.$ix major:$MAJORS{$ix}\n";
    }

    open PROCFILE, $procfile or 
                  die "getBoards:ERROR failed to open procfile $procfile\n";

    my $nboards = 0;

    if ( $slotbased eq "slotbased" ){
	print "Slot based device addressing\n";
    }else{
	print "Resource based device addressing\n";
    }

    my $parser = $slotbased eq "slotbased" ?
		    \&getSlotBoardNum: \&getResourceBoardNum;

    open BUSFILE, "$procfile.bus" or
	die "getBoards:ERROR failed to open $procfile.bus\n";

    while ( $line = <PROCFILE> ){
	my $busline = <BUSFILE>;
	my $refbrec;

	if ( $refbrec = parseBoardRec( $line ) ){
	    processBusLine( $refbrec, $busline );
	    $parser->( $refbrec );
	    push( @DEVRECS, $refbrec );
	    $nboards++;
	}
    }
    close PROCFILE;
    close BUSFILE;

    return $nboards;
}


sub getHeartbeat {
    my ( $refb, $procroot, $model ) = @_;

    my $procfile = $procroot . '/' . $model;

    open PROCFILE, $procfile or 
	die "getBoards:ERROR failed to open procfile $procfile\n";

  PROCLINE: 
    while ( $line = <PROCFILE> ){
	my $testb = parseBoardRec( $line );

	if ( $testb != 0 && $refb->{devnum} == $testb->{devnum} ){
	    $refb->{heartbeat} = $testb->{heartbeat};
	    last PROCLINE;
	}
    }


    close PROCFILE;

    return $refb->{heartbeat};
}


print "getBoards 99\n";
