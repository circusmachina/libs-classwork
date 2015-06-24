{ time.inc: A class to represent a date and time.

  This file is part of classwork.pp, a library of basic and useful classes.

  This program is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 3 of the License or, at your
  option, any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABLITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.

  You should have received a copy of the GNU General Public License along
  with this program.  If not, see <http://www.gnu.org/licenses/>.
}
{$IFDEF ReadInterface}
type TRawTimestamp = double;

{ This class represents a single date and time.  It is defined primarily for
  convenience, in order to provide a way to interact with dates and times that
  is free of the limitations of Free Pascal's built-in @code(TDateTime) type.

  If the default constructor is used (@link(AnObject.new ATimestamp.new)), then
  the instance created will represent the current system date and time.
}
type ATimestamp = class(APrintingObject)
  protected
    // Stores the timestamp -- the number of seconds since the epoch
    myTimestamp: TRawTimestamp;
    { Stores the format used to construct a string representation of the
      timestamp
    }
    myFormat: string;

  public
    { Construct a new instance of @link(ATimestamp) that represents the
      specified date and time.

      @code(thisDateTime) should be an instance of @code(TDateTime), such as
      that returned by @code(SysUtils.encodeDate) or @code(SysUtils.encodeTime).
    }
    constructor new(const thisDateTime: TDateTime); virtual; overload;

    { Construct a new instance of @link(ATimestamp) that represents the
      specified date.

      @param(year can be any signed 64-bit value.)
      @param(month should be a value between 1 and 12.)
      @param(day should be a value between 1 and 31.)
    }
    constructor new(const year: int64; const month, day: word); virtual;
      overload;

    { Construct a new instance of @link(ATimestamp) that represents the
      specified time.

      @param(hour should be a value between 0 (midnight) and 23.)
      @param(minute should be a value between 0 and 59.)
      @param(second, if specified, should be a value between 0 and 59.)
      @param(msecond, if specified, should be a value between 0 and 999.)
    }
    constructor new(const hour, minute: word;
      const second: word = 0; const msecond: word = 0);
      virtual; overload;

    { Construct a new instance of @link(ATimestamp) that represents the
      specified date and time.

      @param(year can be any signed 64-bit value.)
      @param(month should be a value between 1 and 12.)
      @param(day should be a value between 1 and 31.)
      @param(hour should be a value between 0 and 23.)
      @param(minute should be a value between 0 and 59.)
      @param(second, if specified, should be a value between 0 and 59.)
      @param(msecond, if specified, should be a value between 0 and 999.)
    }
    constructor new(const year: int64; const month, day, hour, minute: word;
      const second: word = 0; const msecond: word = 0); virtual; overload;

    { Construct a new instance of @link(ATimestamp) that represents the
      specified date and time.

      @code(thisTimestamp) should be a UNIX-style timestamp: that is, the
      number of seconds which have passed since the UNIX epoch.
    }
    constructor fromUnixTimestamp(const thisTimestamp: int64); virtual;

    // Initializer
    function init: boolean; override;

    { Convert the timestamp to a UNIX-style timestamp. }
    function toUnixTimestamp: int64; virtual;

    { Set the date represented by the timestamp to the current system date,
      without affecting the time of day represented by the timestamp.

      @return(A raw timestamp value that contains the previous date and time
        represented by the timestamp.)
    }
    function setToToday: TRawTimestamp; virtual;

    { Set the date represented by the timestamp to the specified date,
      without affecting the time of day represented by the timestamp.

      Only the date portion of @code(newDate) is used; any time specified is
      ignored.

      @return(A raw timestamp value that contains the previous date and time
        represented by the timestamp.)
    }
    function setDate(const newDate: TDateTime): TRawTimestamp; virtual;
      overload;

    { Set the date represented by the timestamp to the specified date,
      without affecting the time of day represented by the timestamp.

      @param(newYear may be an value that can be contained within a 64-bit
        signed integer.)
      @param(newMonth should be a value between 1 and 12.)
      @param(newDay should be a value between 1 and 31.)

      @return(A raw timestamp value that contains the previous date and time
        represented by the timestamp.)
    }
    function setDate(const newYear: int64;
      const newMonth, newDay: word): TRawTimestamp; virtual; overload;

    { Set the time represented by the timestamp to the current system time,
      without affecting the date represented by the timestamp.

      @return(A raw timestamp value that contains the previous date and time
        represented by the timestamp.)
    }
    function setToNow: TRawTimestamp; virtual;

    { Set the time represented by the timestamp to the specified time, without
      affecting the date represented by the timestamp.

      Only the time portion of @code(newTime) is used; the date, if any, is
      ignored.

      @return(A raw timestamp value that contains the previous date and time
        represented by the timestamp.)
    }
    function setTime(const newTime: TDateTime): TRawTimestamp; virtual;
      overload;

    { Set the time represented by the timestamp to the specified time, without
      affecting the date represented by the timestamp.

      @param(newHour should be a value between 0 and 23.)
      @param(newMinute should be a value between 0 and 59.)
      @param(newSecond, if specified, should be a value between 0 and 59.)
      @param(newMSecond, if specified, should be a value between 0 and 999.)

      @return(A raw timestamp value that contains the previous date and time
        represented by the timestamp.)
    }
    function setTime(const newHour, newMinute: word;
      const newSecond: word = 0; const newMSecond: word = 0): TRawTimestamp;
      virtual; overload;

    { Calculate the number of milliseconds between two timestamp values. }
    function mSecondsSince(const OtherTime: ATimestamp): int64; virtual;

    { Calculate the number of whole seconds between two timestamp values. }
    function secondsSince(const OtherTime: ATimestamp): int64; virtual;

    { Calculate the number of whole minutes between two timestamp values. }
    function minutesSince(const OtherTime: ATimestamp): int64; virtual;

    { Calculate the number of whole hours between two timestamp values. }
    function hoursSince(const OtherTime: ATimestamp): int64; virtual;

    { Calculate the number of whole days between two timestamp values. }
    function daysSince(const OtherTime: ATimestamp): int64; virtual;

    { Calculate the number of whole weeks between two timestamp values. }
    function weeksSince(const OtherTime: ATimestamp): int64; virtual;

    { Calculate the number of whole months between two timestamp values. }
    function monthsSince(const OtherTime: ATimestamp): int64; virtual;

    { Calculate the number of whole years between two timestamp values. }
    function yearsSince(const OtherTime: ATimestamp): int64; virtual;

    { Determine whether the year represented by the timestamp is a leap year. }
    function isLeapYear: boolean; virtual;

    // Properties --------------------------------------------------------------
    { Retrieve the raw timestamp value. }
    function rawValue: TRawTimestamp; virtual;

    { Retrieve the year represented by the timestamp. }
    function year: int64; virtual;

    { Set the year represented by the timestamp.

      @return(The previous value of @link(ATimestamp.year).)
    }
    function setYear(const newYear: int64): int64; virtual;

    { Retrieve the month of the year represented by the timestamp. }
    function month: word; virtual;

    { Set the month of the year represented by the timestamp.

      @code(newMonth) should be a value between @code(1) (January) and @code(12)
      (December).  The value of @code(newMonth) is clamped to the highest
      possible month value allowed; thus, passing values greater than @code(12)
      will result in the timestamp being assigned a value of @code(12).

      @return(The previous value of @link(ATimestamp.month).)
    }
    function setMonth(const newMonth: word): word; virtual;

    { Retrieve the day of the month represented by the timestamp. }
    function day: word; virtual;

    { Set the day of the month represented by the timestamp.

      @code(newDay) should be a value between 1 and 31.  Values greater than the
      number of days defined for the month that is represented by the timestamp
      are clamped to the highest allowed value for that month.  Thus, passing
      a value of @code(30) to a timestamp that represents February will result
      in the timestamp setting a new value of @code(28) instead of @code(30).

      @return(The previous value of @link(ATimestamp.day).)
    }
    function setDay(const newDay: word): word; virtual;

    { Retrieve the hour of the day represented by the timestamp. }
    function hour: word; virtual;

    { Set the hour of the day represented by the timestamp.

      @code(newHour) should be a value between @code(0) (midnight) and
      @code(23).  If @code(newHour) is a value greater than 23, the value is
      clamped to @code(23).

      @return(The previous value of @link(ATimestamp.hour).)
    }
    function setHour(const newHour: word): word; virtual;

    { Retrieve the minute of the hour represented by the timestamp. }
    function minute: word; virtual;

    { Set the minute of the hour represented by the timestamp.

      @code(newMinute) should be a value between @code(0) and @code(59).  If
      @code(newMinute) is greater than @code(59), it is clamped to @code(59).

      @return(The previous value of @link(ATimestamp.minute).)
    }
    function setMinute(const newMinute: word): word; virtual;

    { Retrieve the second of the minute represented by the timestamp. }
    function second: word; virtual;

    { Set the second of the minute represented by the timestamp.

      @code(newSecond) should be a value between @code(0) and @code(59).  If
      @code(newSecond) is greater than @code(59), it is clamped to @code(59).

      @return(The previous value of @link(ATimestamp.second).)
    }
    function setSecond(const newSecond: word): word; virtual;

    { Retrieve the millisecond of the second represented by the timestamp. }
    function mSecond: word; virtual;

    { Set the millisecond of the second represented by the timestamp.

      @code(newMSecond) should be a value between @code(0) and @code(999).  If
      @code(newMSecond) is greater than @code(999), it is clamped to @code(999).

      @return(The previous value of @link(ATimestamp.msecond).)
    }
    function setMSecond(const newMSecond: word): word; virtual;
end;

{$ENDIF}
{$IFDEF ReadImplementation}
const
  // The number of milliseconds in a second
  MSECS_PER_SECOND = 1000;
  // The number of seconds in a minute
  SECS_PER_MINUTE = 60;
  // The number of minutes per hour
  MINS_PER_HOUR = 60;
  // The number of hours per day
  HOURS_PER_DAY = 24;
  // The number of days in a week
  DAYS_PER_WEEK = 7;

  // The maximum number of days per month
  DAYS_PER_MONTH_JANUARY = 31;
  DAYS_PER_MONTH_FEBRUARY = 28;
  DAYS_PER_MONTH_FEBRUARY_LEAP = DAYS_PER_MONTH_FEBRUARY + 1;
  DAYS_PER_MONTH_MARCH = 31;
  DAYS_PER_MONTH_APRIL = 30;
  DAYS_PER_MONTH_MAY = 31;
  DAYS_PER_MONTH_JUNE = 30;
  DAYS_PER_MONTH_JULY = 31;
  DAYS_PER_MONTH_AUGUST = 31;
  DAYS_PER_MONTH_SEPTEMBER = 30;
  DAYS_PER_MONTH_OCTOBER = 31;
  DAYS_PER_MONTH_NOVEMBER = 30;
  DAYS_PER_MONTH_DECEMBER = 31;

  // The maximum number of days per year
  DAYS_PER_YEAR = DAYS_PER_MONTH_JANUARY + DAYS_PER_MONTH_FEBRUARY +
    DAYS_PER_MONTH_MARCH + DAYS_PER_MONTH_APRIL + DAYS_PER_MONTH_MAY +
    DAYS_PER_MONTH_JUNE + DAYS_PER_MONTH_JULY + DAYS_PER_MONTH_AUGUST +
    DAYS_PER_MONTH_SEPTEMBER + DAYS_PER_MONTH_OCTOBER +
    DAYS_PER_MONTH_NOVEMBER + DAYS_PER_MONTH_DECEMBER;

  // The maximum number of days per leap year
  DAYS_PER_LEAP_YEAR = DAYS_PER_YEAR + 1;

  // The number of seconds in a day
  SECS_PER_DAY = SECS_PER_MINUTE * MINS_PER_HOUR * HOURS_PER_DAY;
  // The number of seconds per week
  SECS_PER_WEEK = SECS_PER_DAY * DAYS_PER_WEEK;
  // The number of seconds per year
  SECS_PER_YEAR = SECS_PER_DAY * DAYS_PER_YEAR;
  // The number of seconds per leap year
  SECS_PER_LEAP_YEAR = SECS_PER_DAY * DAYS_PER_LEAP_YEAR;

  // Quick lookup of the number of days per month
  DAYS_PER_MONTH: array[MONTH_JANUARY..MONTH_DECEMBER] of word = (
    DAYS_PER_MONTH_JANUARY,
    DAYS_PER_MONTH_FEBRUARY,
    DAYS_PER_MONTH_MARCH,
    DAYS_PER_MONTH_APRIL,
    DAYS_PER_MONTH_MAY,
    DAYS_PER_MONTH_JUNE,
    DAYS_PER_MONTH_JULY,
    DAYS_PER_MONTH_AUGUST,
    DAYS_PER_MONTH_SEPTEMBER,
    DAYS_PER_MONTH_OCTOBER,
    DAYS_PER_MONTH_NOVEMBER,
    DAYS_PER_MONTH_DECEMBER
  );

// ATimestamp ******************************************************************


{$ENDIF}
