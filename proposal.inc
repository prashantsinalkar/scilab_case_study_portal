<?php
function scilab_case_study_proposal_all()
{
    global $user;
    $page_content = "";
    if (!$user->uid)
    {
        $page_content .= "<ul>";
        $page_content .= "<li>Please <a href='/user'><b><u>Login</u></b></a> to create a proposal.</li>";
        $page_content .= "</ul>";
        return $page_content;
    } //!$user->uid
    /* check if user has already submitted a proposal */
    $proposal_q = db_query("SELECT * FROM {scilab_case_study_proposal} WHERE uid = :uid ORDER BY id DESC LIMIT 1", array(":uid" =>$user->uid));
    if ($proposal_q)
    {
        if ($proposal_data = $proposal_q->fetchObject())
        {
            switch ($proposal_data->proposal_status)
            {
                case 0:
                    drupal_set_message(t('We have already received your scilab case study proposal. We will get back to you soon.'), 'status');
                    drupal_goto('');
                    return;
                    break;
                case 1:
                    drupal_set_message(t('Your proposal has been approved. Please go to ' . l('Code Submission', 'textbook_companion/code') . ' to upload your code'), 'status');
                    drupal_goto('');
                    return;
                    break;
                case 2:
                    drupal_set_message(t('Your scilab case study proposal has been dis-approved. Please create another scilab case study proposal below.'), 'error');
                    break;
                case 3:
                    drupal_set_message(t('Congratulations! You have completed your last scilab case study proposal. You can create another scilab case study proposal below.'), 'status');
                    break;
                default:
                    drupal_set_message(t('Invalid scilab case study proposal state. Please contact site administrator for further information.'), 'error');
                    drupal_goto('');
                    return;
                    break;
            } //$proposal_data->proposal_status
        } //$proposal_data = $proposal_q->fetchObject()
    } //$proposal_q
    $scilab_case_study_proposal_form = drupal_get_form("scilab_case_study_proposal_form");
    $page_content .= drupal_render($scilab_case_study_proposal_form);
    return $page_content;
}
function scilab_case_study_proposal_form($form, &$form_state)
{
    global $user;
    $form = array();
    $form['imp_notice'] = array(
        '#type' => 'item',
        '#markup' => '<font color="red"><b>Please fill up this form carefully as the details entered here will be exactly written in the Scilab case study proposal and also follow the additional guidelines.</b></font>'
    );
    $form['guidelines'] = array(
        '#type' => 'fieldset',
        '#title' => t('Guidelines'),
        '#attributes' => array(
            'style' => 'font-weight: bold'
        ),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE
    );
    $form['guidelines']['book'] = array(
        '#type' => 'item',
        '#required' => TRUE,
        '#markup' => '<ul style="list-style-type:disc;font-weight: normal">
               <li>All the fields are compulsory</li>
               <li>Make sure the Scilab case study you propose is not already a <a href="#" target="_blank">Scilab case study in progress</a> </li>
               <li>You will be intimated about the approval or rejection of your suggestion via e-mail</li>
</ul> '
    );
    $form['full_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Full Name'),
        '#size' => 30,
        '#maxlength' => 50,
        '#required' => TRUE
    );
    $form['email_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#size' => 30,
        '#value' => $user->mail,
        '#disabled' => TRUE
    );
    $form['mobile'] = array(
        '#type' => 'textfield',
        '#title' => t('Mobile No.'),
        '#size' => 30,
        '#maxlength' => 15,
        '#required' => TRUE
    );
    $form['gender'] = array(
        '#type' => 'radios',
        '#title' => t('Gender'),
        '#options' => array(
            'M' => 'Male',
            'F' => 'Female',
            'O' => 'Other'
        ),
        '#required' => TRUE
    );
    $form['course'] = array(
        '#type' => 'textfield',
        '#title' => t('Course'),
        '#size' => 30,
        '#maxlength' => 50,
        '#required' => TRUE
    );
    $form['branch'] = array(
        '#type' => 'select',
        '#title' => t('Department/Branch'),
        '#options' => _tbc_list_of_departments(),
        '#required' => TRUE
    );
    $form['university'] = array(
        '#type' => 'textfield',
        '#title' => t('University/Institute'),
        '#size' => 30,
        '#maxlength' => 50,
        '#required' => TRUE
    );
    $form['country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array(
            'India' => 'India',
            'Others' => 'Others'
        ),
        '#required' => TRUE,
        '#tree' => TRUE,
        '#validated' => TRUE
    );
    $form['other_country'] = array(
        '#type' => 'textfield',
        '#title' => t('Other than India'),
        '#size' => 100,
        '#attributes' => array(
            'placeholder' => t('Enter your country name')
        ),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others'
                )
            )
        )
    );
    $form['other_state'] = array(
        '#type' => 'textfield',
        '#title' => t('State other than India'),
        '#size' => 100,
        '#attributes' => array(
            'placeholder' => t('Enter your state/region name')
        ),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others'
                )
            )
        )
    );
    $form['other_city'] = array(
        '#type' => 'textfield',
        '#title' => t('City other than India'),
        '#size' => 100,
        '#attributes' => array(
            'placeholder' => t('Enter your city name')
        ),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others'
                )
            )
        )
    );
    $form['all_state'] = array(
        '#type' => 'select',
        '#title' => t('State'),
        '#options' => _tbc_list_of_states(),
        '#validated' => TRUE,
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'India'
                )
            )
        )
    );
    $form['city'] = array(
        '#type' => 'select',
        '#title' => t('City'),
        '#options' => _tbc_list_of_cities(),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'India'
                )
            )
        )
    );
    $form['pincode'] = array(
        '#type' => 'textfield',
        '#title' => t('Pincode'),
        '#size' => 30,
        '#maxlength' => 6,
        '#required' => False,
        '#attributes' => array(
            'placeholder' => 'Enter pincode....'
        )
    );
    $form['version'] = array(
        '#type' => 'textfield',
        '#title' => t('Scilab Version'),
        '#description' => t('Input version format should be like 5.4 or 5.4.1'),
        '#size' => 10,
        '#required' => TRUE,
        '#maxlength' => 20
    );
    $form['completion_date'] = array(
        '#type' => 'textfield',
        '#title' => t('Expected Date of Completion'),
        '#description' => t('Input date format should be DD-MM-YYYY. Eg: 23-03-2011'),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => TRUE
    );
    $form['operating_system'] = array(
        '#type' => 'textfield',
        '#title' => t('Operating System'),
        '#required' => TRUE,
        '#size' => 30,
        '#maxlength' => 50
    );
    $form['domain_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Domain Name'),
        '#required' => TRUE,
        '#size' => 100,
        '#maxlength' => 100
    );
    $form['samplefile'] = array(
        '#type' => 'fieldset',
        '#title' => t('<span class="form-required form-item" title="This field is required.">Abstract of case study Files *</span>'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE
    );
    $form['samplefile']['samplefile1'] = array(
        '#type' => 'file',
        '#title' => t('Abstract of case study'),
        '#size' => 48,
        '#description' => t('Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('textbook_companion_sample_source_file_extensions', '') . '</span>'
    );
    $form['termconditions'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Terms And Conditions'),
        '#options' => array(
            'status' => t('<a href="term-and-conditions" target="_blank">I agree to the Terms and Conditions</a>')
        ),
        '#required' => TRUE
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit')
    );
    return $form;
}
