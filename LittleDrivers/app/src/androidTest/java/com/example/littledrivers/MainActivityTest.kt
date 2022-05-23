package com.example.littledrivers

import androidx.test.internal.runner.junit4.AndroidJUnit4ClassRunner
import androidx.test.core.app.ActivityScenario
import androidx.test.espresso.Espresso
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.action.ViewActions.click
import androidx.test.espresso.assertion.ViewAssertions
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.matcher.ViewMatchers
import androidx.test.espresso.matcher.ViewMatchers.isDisplayed
import androidx.test.espresso.matcher.ViewMatchers.withId
import com.example.littledrivers.MainActivity
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4ClassRunner::class)
class MainActivityTest {
    @Test
    fun test_navHomePage_theme1() {
        val activityScenario = ActivityScenario.launch(HomeActivity::class.java)

        onView(withId(R.id.Theme1)).perform(click())
        onView(withId(R.id.Theme1Activity)).check(matches(isDisplayed()))
    }

    @Test
    fun theme1_back_to_HomePage() {
        val activityScenario = ActivityScenario.launch(Theme1Activity::class.java)

        onView(withId(R.id.home1)).perform(click())
        onView(withId(R.id.homeActivity)).check((matches(isDisplayed())))
    }

   /* @Test
    fun test_navHomePage_theme2() {
        val activityScenario = ActivityScenario.launch(HomeActivity::class.java)

        onView(withId(R.id.Theme2)).perform(click())
        onView(withId(R.id.Theme2Activity)).check(matches(isDisplayed()))
    }*/

    @Test
    fun theme2_back_to_HomePage() {
        val activityScenario = ActivityScenario.launch(Theme2Activity::class.java)

        onView(withId(R.id.home2)).perform(click())
        onView(withId(R.id.homeActivity)).check((matches(isDisplayed())))
    }

   /* @Test
    fun test_navHomePage_theme3() {
        val activityScenario = ActivityScenario.launch(HomeActivity::class.java)

        onView(withId(R.id.Theme3)).perform(click())
        onView(withId(R.id.Theme3Activity)).check(matches(isDisplayed()))
    }*/

    @Test
    fun theme3_back_to_HomePage() {
        val activityScenario = ActivityScenario.launch(Theme3Activity::class.java)

        onView(withId(R.id.home3)).perform(click())
        onView(withId(R.id.homeActivity)).check((matches(isDisplayed())))
    }

   /* @Test
    fun test_navHomePage_theme4() {
        val activityScenario = ActivityScenario.launch(HomeActivity::class.java)

        onView(withId(R.id.Theme4)).perform(click())
        onView(withId(R.id.Theme4Activity)).check(matches(isDisplayed()))
    }*/

    @Test
    fun theme4_back_to_HomePage() {
        val activityScenario = ActivityScenario.launch(Theme4Activity::class.java)

        onView(withId(R.id.home4)).perform(click())
        onView(withId(R.id.homeActivity)).check((matches(isDisplayed())))
    }



}