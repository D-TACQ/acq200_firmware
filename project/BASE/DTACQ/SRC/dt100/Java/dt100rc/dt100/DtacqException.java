package dt100;

class DtacqException extends RuntimeException {
    protected DtacqException() {}
    public DtacqException( String msg ) {
        super( msg );
    }
}