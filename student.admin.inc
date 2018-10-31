<?php

/**
 * @file
 * Provide form definition for student module.
 */

/**
 * Implements hook_form().
 */
function student_form($form, &$form_state) {
    $form['st_fnm'] = array(
    '#type' => 'textfield',
      '#title' => t('Student First Name:'),
      '#size' => 25,
      '#required' => TRUE,
      );
    $form['st_lnm'] = array(
    '#type' => 'textfield',
    '#title' => t('Student Last Name:'),
    '#size' => 25,
    '#required' => TRUE,
      );
    $form['st_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Student Email:'),
    '#size' => 25,
    '#required' => TRUE,
      );
    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
      );
    return $form;
}

/**
 * Implements hook_validate().
 */
function student_form_validate($form, &$form_state) {
    $email = check_plain($form_state['values']['st_email']);
    $result = db_select('student', 'st')
    ->fields('st', array('st_email'))
    ->condition('st_email', $email)
    ->countQuery()->execute()->fetchField();
    if ($result > 0) {
        form_set_error('st_email', 'Email  already exists');
    }
}

/**
 * Implements hook_submit().
 */
function student_form_submit($form, &$form_state) {
    $st_id = db_insert('student')
    ->fields(array(
      'st_fnm' => check_plain($form_state['values']['st_fnm']),
      'st_lnm' => check_plain($form_state['values']['st_lnm']),
      'st_email' => check_plain($form_state['values']['st_email']),
    ))
    ->execute();
    if (!$st_id) {
        drupal_set_message(t('Error'));
    }
    else {
        drupal_set_message(t('Entry has been added.'));
    }
}

/**
 * Displaying student information in Form list page of page.
 */
function student_list() {
    $query = db_select('student', 'st');
    $query
    ->fields('st', array('st_id', 'st_fnm', 'st_lnm', 'st_email'))
    ->range(0, 50)
    ->orderby('st . st_id');
    $results = $query->execute();
    $header[] = t('ID');
    $header[] = t('Student First Name');
    $header[] = t('Student Last Name');
    $header[] = t('E-mail');
    $header[] = t('Delete');
    $header[] = t('Edit');
    $rows = array();
    foreach ($results as $result) {
        $rows[] = array(
        check_plain($result->st_id),
        check_plain($result->st_fnm),
        check_plain($result->st_lnm),
        check_plain($result->st_email),
          l(t('Delete'), 'student/records/' . $result->st_id . '/delete'),
          l(t('Edit'), 'student/records/' . $result->st_id . '/edit'),
          );
    }
    return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Deleting student records.
 */
function student_form_delete() {
    $id = arg(2);
    $num_deleted = db_delete('student')
    ->condition('st_id', $id)
    ->execute();
    if ($num_deleted) {
        drupal_set_message(t('Student records  has been deleted.'));
        drupal_goto("student/records");
    }
    else {
        drupal_set_message(t('Error in query'));
    }
}

/**
 * Editing the Student record information.
 */
function student_edit_form() {
    $form = array();
    $id = arg(2);
    $result = db_select('student', 'st')
    ->fields('st', array('st_fnm', 'st_lnm', 'st_email'))
    ->condition('st_id', $id)
    ->execute()->fetchObject();
    $form['st_fnm'] = array(
    '#type' => 'textfield',
    '#title' => t('Student First Name:'),
    '#size' => 25,
      '#required' => TRUE,
    '#value' => $result->st_fnm,
      );
    $form['st_lnm'] = array(
    '#type' => 'textfield',
    '#title' => t('Student Last Name:'),
    '#size' => 25,
    '#required' => TRUE,
    '#value' => $result->st_lnm,
      );
    $form['st_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
    '#size' => 25,
      '#required' => TRUE,
    '#value' => $result->st_email,
      );
    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('update'),
      );
    return $form;
}

/**
 * Submitting Edit form using hook_submit.
 */
function student_edit_form_submit($form, &$form_state) {
    $id = arg(2);
    $st_id = db_update('student')
    ->fields(array(
      'st_fnm' => check_plain($form_state['input']['st_fnm']),
      'st_lnm' => check_plain($form_state['input']['st_lnm']),
      'st_email' => check_plain($form_state['input']['st_email']),
    ))
    ->condition('st_id', $id)
    ->execute();
    drupal_set_message(t('Student records  has been updated.'));
}
