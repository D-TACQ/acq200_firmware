package fileconverter;

import javax.swing.filechooser.FileFilter;
import java.io.File;

class DefaultFileFilter extends FileFilter {
    private final String extension;
    private final String description;

    public DefaultFileFilter( String extension, String description ) {
        this.extension = extension;
        this.description = description;
    }
    public boolean accept( File f ) {
        return f.getName().toLowerCase().endsWith( extension ) || f.isDirectory();
    }
    public String getDescription() {
        return description;
    }
    protected File getFullFile( File file ) {
        if ( file.getName().endsWith( extension ) ){
            return file;                                // we have the extension already
        }else{
            return new File( file.getParent(), file.getName()+extension );
        }
    }
}
