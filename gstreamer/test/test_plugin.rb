class TestPlugin < Test::Unit::TestCase
  def test_source
    plugin = Gst::Registry.default.find_plugin("playback")
    assert_equal("gst-plugins-base", plugin.source)
  end
end
