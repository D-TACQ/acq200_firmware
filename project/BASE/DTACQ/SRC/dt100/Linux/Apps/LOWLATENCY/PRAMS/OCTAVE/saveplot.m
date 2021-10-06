
function saveplot(xx, ename, range, path)
	if (nargin != 4)
		usage("var, ename, range, path");
	endif
	gset term postscript color;
	outfilecommand = sprintf("gset output \"%s\"", path);
	eval(outfilecommand);
	plotcmd = sprintf("%s=xx;plot(%s)", ename, range);
	titlecmd = sprintf("gset title \"%s\"", range);
	eval(titlecmd);
	gset timestamp
	eval(plotcmd);
	gset output;gset term x11;
endfunction
