require 'socket'

class NetworkedBasilTest
  def initialize(host, port)
    @host = host
    @port = port
    @sock = UDPSocket.open
  end

  def test_led(message)
    @sock.send(message, 0, @host, @port)
  end
end
