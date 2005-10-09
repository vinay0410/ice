// **********************************************************************
//
// Copyright (c) 2003-2005 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
package IceGrid.TreeNode;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextField;

import com.jgoodies.forms.builder.DefaultFormBuilder;

import IceGrid.Model;
import IceGrid.NodeDescriptor;
import IceGrid.TableDialog;
import IceGrid.Utils;

class NodeEditor extends Editor
{
    protected void applyUpdate()
    {
    }

    NodeEditor(JFrame parentFrame)
    {
	_name.getDocument().addDocumentListener(_updateListener);
	_variables.setEditable(false);

	//
	// Variables
	//
	_variablesDialog = new TableDialog(parentFrame, 
					   "Variables", "Name", "Value", true);
	
	Action openVariablesDialog = new AbstractAction("...")
	    {
		public void actionPerformed(ActionEvent e) 
		{
		    java.util.TreeMap result = _variablesDialog.show(_variablesMap, 
								     getPanel());
		    if(result != null)
		    {
			updated();
			_variablesMap = result;
			setVariablesField();
		    }
		}
	    };
	_variablesButton = new JButton(openVariablesDialog);

	_loadFactor.getDocument().addDocumentListener(_updateListener);
    }
 
    void append(DefaultFormBuilder builder)
    {    
	builder.append("Name");
	builder.append(_name, 3);
	builder.nextLine();
	builder.append("Variables", _variables);
	builder.append(_variablesButton);
	builder.nextLine();
    }
    
    boolean isSimpleUpdate()
    {
	NodeDescriptor descriptor = (NodeDescriptor)_target.getDescriptor();
	return (_variablesMap.equals(descriptor.variables));
    }

    void writeDescriptor()
    {
	NodeDescriptor descriptor = (NodeDescriptor)_target.getDescriptor();
	descriptor.variables = _variablesMap;
	descriptor.loadFactor = _loadFactor.getText();
    }	    
    

    void show(Node node)
    {
	detectUpdates(false);
	setTarget(node);
	
	_name.setText(_target.getId());
	_name.setEditable(_target.isEphemeral());
	
	NodeDescriptor descriptor = (NodeDescriptor)_target.getDescriptor();

	_variablesMap = descriptor.variables;
	setVariablesField();
	
	_loadFactor.setText(descriptor.loadFactor);

	_applyButton.setEnabled(node.isEphemeral());
	_discardButton.setEnabled(node.isEphemeral());	  
	detectUpdates(true);
    }

    private void setVariablesField()
    {
	Ice.StringHolder toolTipHolder = new Ice.StringHolder();
	_variables.setText(
	    Utils.stringify(_variablesMap, "=", ", ", toolTipHolder));
	_variables.setToolTipText(toolTipHolder.value);
    }

    
    private JTextField _name = new JTextField(20);
    private JTextField _variables = new JTextField(20);
    private JButton _variablesButton;
    private TableDialog _variablesDialog;
    private java.util.TreeMap _variablesMap;
    private JTextField _loadFactor = new JTextField(20);

}
