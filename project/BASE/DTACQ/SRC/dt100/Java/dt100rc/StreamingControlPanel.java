
import dtacqserver.ReadWriteStream;
import dtacqserver.DtacqClient;

import javax.swing.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.*;
import java.io.*;

import resources.DtacqComboBox;
import resources.UiState;
import dt100.StreamingAdapter;
import dt100.StreamingCallbackManager;
import dt100.Dt100;

/**
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: Jun 27, 2004
 * Time: 10:18:52 AM
 * To change this template use File | Settings | File Templates.
 */


public class StreamingControlPanel extends JPanel {
    private JButton runStop;
    private JTextField oneSecondPort;
    private JTextField oneSecondHost;
    private JTextField jobDef;

    public JButton getRunStop() {
        return runStop;
    }

    public JButton getSetRootButton() {
        return setRootButton;
    }

    public JButton getConnectOneSecondServer() {
        return connectOneSecondServer;
    }

    public JTextField getRootDirectory() {
        return rootDirectory;
    }

    public JCheckBox getSaveRootCheckBox() {
        return saveRootCheckBox;
    }

    public JCheckBox getOnSecondCheckbox() {
        return onSecondCheckbox;
    }

    public JCheckBox getMfCheckbox() {
        return mfCheckbox;
    }

    public JComboBox getStreamSubrate() {
        return streamSubrate;
    }

    public JComboBox getStreamMaxChannels() {
        return streamMaxChannels;
    }

    private JButton setRootButton;
    private JButton connectOneSecondServer;
    private JTextField rootDirectory;
    private JCheckBox saveRootCheckBox;
    private JCheckBox onSecondCheckbox;
    private JCheckBox mfCheckbox;
    private JComboBox streamSubrate;
    private JComboBox streamMaxChannels;
    private JPanel panel;

    Dt100 dt100;
    ReadWriteStream oneSecondConnection;

    private void makeOneSecondConnection() {
        System.err.println("makeOneSecondConnection");

        if (oneSecondConnection == null){
            try {
                oneSecondConnection = DtacqClient.connectRaw(
                        oneSecondHost.getText(),
                        Integer.parseInt(oneSecondPort.getText()));
                connectOneSecondServer.setText("Drop");
            } catch (IOException e) {
                System.err.println("ERROR:makeOneSecondConnection failed to connect " +
                            oneSecondHost.getText() + ":" + oneSecondPort.getText() + "\n" + e);
                return;
            }
        }

    }
    private void dropOneSecondConnection() {
        if (oneSecondConnection != null){
            DtacqClient.close(oneSecondConnection);
            connectOneSecondServer.setText("Connect");
            oneSecondConnection = null;
            System.err.println("dropOneSecondConnection");
        }
    }
    private StreamingControlPanel() {
        add(panel);
        runStop.setToolTipText("Start streaming data");
        streamSubrate.setToolTipText("Set Decimation factor on full rate capture");
        streamMaxChannels.setToolTipText("Set Number of channels in stream data set");
        mfCheckbox.setToolTipText("Allow multiframe streaming");

        connectOneSecondServer.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (connectOneSecondServer.getText().startsWith("Connect")){
                    makeOneSecondConnection();
                }else{
                    dropOneSecondConnection();
                }
            }

        });
        runStop.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (runStop.getText().equals("Start") && MainFrame.getInstance().onStartStreaming()){
                    if (onSecondCheckbox.isSelected() && oneSecondConnection == null){
                        makeOneSecondConnection();
                    }
                    startStreaming();
                }else{
                    MainFrame.getInstance().onStopStreaming();
                    stopStreaming();
                }
            }
        });
        MultiframeStatsPanel.getInstance().setClient_endCB(new Runnable(){
            public void run() {
                if (oneSecondConnection != null){
                    PrintWriter output = oneSecondConnection.getOutput();
                    final MultiframeStatsPanel msp = MultiframeStatsPanel.getInstance();
                    output.println("waitfor " + msp.getJiffies() + " " +
                                jobDef.getText() + " " +  msp.getPreviousSecond());
                }
            }
        });
    }


    public void loadState(ObjectInputStream in) throws IOException, ClassNotFoundException {
        getSaveRootCheckBox().setSelected( ((JCheckBox)in.readObject()).isSelected());
        getMfCheckbox().setSelected(((JCheckBox)in.readObject()).isSelected());
        onSecondCheckbox.setSelected(((JCheckBox)in.readObject()).isSelected());
        oneSecondHost.setText((String)in.readObject());
        oneSecondPort.setText((String)in.readObject());
        jobDef.setText((String)in.readObject());
    }
    public void saveState(ObjectOutputStream out) throws IOException {
        out.writeObject(getSaveRootCheckBox());
        out.writeObject(getMfCheckbox());
        out.writeObject(onSecondCheckbox);
        out.writeObject(oneSecondHost.getText());
        out.writeObject(oneSecondPort.getText());
        out.writeObject(jobDef.getText());
    }
    public void startStreaming() {
            if (getMfCheckbox().isSelected()){
                MultiframeStatsPanel.getInstance().onStreamStart();
            }

            dt100.startStreaming(dt100.getMaster(), // WORKTODO !!! only MASTER?
                    Integer.parseInt(streamSubrate.getSelectedItem().toString()),
                    Integer.parseInt(streamMaxChannels.getSelectedItem().toString()),
                    mfCheckbox.isSelected(),
                    getSaveRootCheckBox().isSelected() ?
                    getRootDirectory().getText() : null);

            runStop.setText("Stop");
    }
    public void stopStreaming() {
        if (getMfCheckbox().isSelected()){
            MultiframeStatsPanel.getInstance().onStreamStop();
        }
        MainFrame.getInstance().onStopStreaming();
        runStop.setText("Start");
    }

    public void setState(UiState uistate, Dt100 dt100){
        this.dt100 = dt100;
        streamMaxChannels.setModel(uistate.streamMaxChannelsModel);
        streamSubrate.setModel(uistate.streamSubrateModel);
    }

    private static StreamingControlPanel instance;

    public static StreamingControlPanel createStreamingControlPanel() {
        if (instance == null){
            instance = new StreamingControlPanel();
        }
        return instance;
    }

    public static StreamingControlPanel getInstance() {
        return instance;
    }

    {
// GUI initializer generated by IntelliJ IDEA GUI Designer
// !!! IMPORTANT !!!
// DO NOT EDIT OR ADD ANY CODE HERE!
        $$$setupUI$$$();
    }

    /**
     * Method generated by IntelliJ IDEA GUI Designer
     * !!! IMPORTANT !!!
     * DO NOT edit this method OR call it in your code!
     */
    private void $$$setupUI$$$() {
        panel = new JPanel();
        panel.setLayout(new com.intellij.uiDesigner.core.GridLayoutManager(11, 3, new Insets(5, 5, 5, 5), -1, -1));
        panel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEtchedBorder(), "Control Live Data Streaming"));
        streamMaxChannels = new JComboBox();
        streamMaxChannels.setEnabled(true);
        panel.add(streamMaxChannels, new com.intellij.uiDesigner.core.GridConstraints(1, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(50, -1), null));
        streamSubrate = new JComboBox();
        streamSubrate.setEnabled(true);
        panel.add(streamSubrate, new com.intellij.uiDesigner.core.GridConstraints(1, 1, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, new Dimension(50, -1), null, null));
        final JLabel label1 = new JLabel();
        label1.setText("Max Channels");
        panel.add(label1, new com.intellij.uiDesigner.core.GridConstraints(0, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        final JLabel label2 = new JLabel();
        label2.setText("Subrate");
        panel.add(label2, new com.intellij.uiDesigner.core.GridConstraints(0, 1, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        mfCheckbox = new JCheckBox();
        mfCheckbox.setEnabled(true);
        mfCheckbox.setText("MultiFrame");
        panel.add(mfCheckbox, new com.intellij.uiDesigner.core.GridConstraints(2, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_SHRINK | com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        saveRootCheckBox = new JCheckBox();
        saveRootCheckBox.setEnabled(true);
        saveRootCheckBox.setText("Save Root");
        panel.add(saveRootCheckBox, new com.intellij.uiDesigner.core.GridConstraints(3, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_SHRINK | com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        rootDirectory = new JTextField();
        panel.add(rootDirectory, new com.intellij.uiDesigner.core.GridConstraints(4, 0, 1, 2, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_HORIZONTAL, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_WANT_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(400, -1), null));
        setRootButton = new JButton();
        setRootButton.setText("Set Root...");
        panel.add(setRootButton, new com.intellij.uiDesigner.core.GridConstraints(4, 2, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_CENTER, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_SHRINK | com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        onSecondCheckbox = new JCheckBox();
        onSecondCheckbox.setText("On Second Processing");
        panel.add(onSecondCheckbox, new com.intellij.uiDesigner.core.GridConstraints(5, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_SHRINK | com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        final JLabel label3 = new JLabel();
        label3.setText("Host");
        panel.add(label3, new com.intellij.uiDesigner.core.GridConstraints(6, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        oneSecondHost = new JTextField();
        oneSecondHost.setText("localhost");
        panel.add(oneSecondHost, new com.intellij.uiDesigner.core.GridConstraints(7, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_HORIZONTAL, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_WANT_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null));
        oneSecondPort = new JTextField();
        oneSecondPort.setText("53506");
        panel.add(oneSecondPort, new com.intellij.uiDesigner.core.GridConstraints(7, 1, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_HORIZONTAL, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_WANT_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(75, -1), null));
        connectOneSecondServer = new JButton();
        connectOneSecondServer.setText("Connect");
        panel.add(connectOneSecondServer, new com.intellij.uiDesigner.core.GridConstraints(7, 2, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_CENTER, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_SHRINK | com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        final JLabel label4 = new JLabel();
        label4.setText("Job");
        panel.add(label4, new com.intellij.uiDesigner.core.GridConstraints(8, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        final JLabel label5 = new JLabel();
        label5.setText("Port");
        panel.add(label5, new com.intellij.uiDesigner.core.GridConstraints(6, 1, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        runStop = new JButton();
        runStop.setEnabled(true);
        runStop.setText("Start");
        panel.add(runStop, new com.intellij.uiDesigner.core.GridConstraints(10, 2, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_CENTER, com.intellij.uiDesigner.core.GridConstraints.FILL_NONE, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_SHRINK | com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_CAN_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, null, null));
        jobDef = new JTextField();
        jobDef.setText("job");
        panel.add(jobDef, new com.intellij.uiDesigner.core.GridConstraints(9, 0, 1, 1, com.intellij.uiDesigner.core.GridConstraints.ANCHOR_WEST, com.intellij.uiDesigner.core.GridConstraints.FILL_HORIZONTAL, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_WANT_GROW, com.intellij.uiDesigner.core.GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null));
    }
}
