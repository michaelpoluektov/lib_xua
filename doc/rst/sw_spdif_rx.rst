|newpage|

S/PDIF Receive
==============

XMOS devices can support S/PDIF receive up to 192kHz - see `lib_spdif <https://www.xmos.com/file/lib_spdif>`__ for full specifications.

The S/PDIF receiver module uses a clock-block and a buffered one-bit port.
The clock-block is divided off a 100 MHz reference clock. The one bit port is buffered to 32 bits.
The receiver code uses this clock to over sample the input data.

The receiver outputs audio samples over a *streaming channel end* where data can be input using the
built-in input operator. ``lib_spdif`` also provides API functions that wrap up this communication.

The S/PDIF receive function never returns. The 32-bit value from the channel input comprises of fields shown in :ref:`spdif_rx_word_structure`.

.. _spdif_rx_word_structure:

.. list-table:: S/PDIF RX Word Structure
     :header-rows: 1
     :widths: 10 32

     * - Bits
       -
     * - 0:3
       - A tag (see below)
     * - 4:28
       - PCM encoded sample value
     * - 29:31
       - User bits (parity, etc)

The tag has one of three values, as shown in :ref:`spdif_rx_tag`.

.. _spdif_rx_tag:

.. list-table:: S/PDIF RX Tags
     :header-rows: 1
     :widths: 10 32

     * - Tag
       - Meaning
     * - FRAME\_X
       - Sample on channel 0 (Left for stereo)
     * - FRAME\_Y
       - Sample on another channel (Right if for stereo)
     * - FRAME\_Z
       - Sample on channel 0 (Left), and the first sample of a frame; can be used if the user bits need to be reconstructed.

See S/PDIF, IEC 60958-3:2006, specification for further details on format, user bits etc.

Usage and Integration
---------------------

Since S/PDIF is a digital stream, the device's master clock must be synchronised to it. This is typically
done with an external device. See `Clock Recovery` (:ref:`usb_audio_sec_clock_recovery`).

.. note::

   Due to the requirement for this clock recovery S/PDIF receive can only be used in Asynchronous
   mode.

The S/PDIF receive function communicates with the Clock Gen thread, which in turn passes audio data to the
Audio Hub thread. The Clock Gen thread also handles locking to the S/PDIF clock source
(see :ref:`usb_audio_sec_clock_recovery`).

Ideally the parity of each word/sample received should be checked.  This is done using the built in
``crc32`` function (see ``xs1.h``):

.. literalinclude:: ../../lib_xua/src/core/clocking/clockgen.xc
  :start-after: /* Check parity and ignore if bad */
  :end-before: /* Get preamble */

If bad parity is detected the word/sample is ignored, otherwise the tag is inspected for channel
(i.e. left or right) and the sample stored.

The following code snippet illustrates how the output of the S/PDIF receive component could is used::

  while(1)
  {
     c_spdif_rx :> data;

     if(badParity(data)
       continue;

     tag = data & 0xF;

     /* Extract 24bit audio sample */
     sample = (data << 4) & 0xFFFFFF00;

     switch(tag)
     {
       case FRAME_X:
       case FRAME_X:
         // Store left
         break;

       case FRAME_Z:
         // Store right
         break;
     }
  }

The Clock Gen thread stores samples in a small FIFO before they are communicated to the Audio Hub thread.

