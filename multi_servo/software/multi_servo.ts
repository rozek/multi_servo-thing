import Thing from "../../../src/lib/thing"

export default class multi_servo extends Thing {
  async setRGB (R:number, G:number, B:number):Promise<void> {
    const LED_R = Math.floor(65535 * Math.max(0,Math.min(R,1)))
    const LED_G = Math.floor(65535 * Math.max(0,Math.min(G,1)))
    const LED_B = Math.floor(65535 * Math.max(0,Math.min(B,1)))

    const Datagram = new Uint8Array([
      LED_R & 0xFF, (LED_R >> 8) & 0xFF,
      LED_G & 0xFF, (LED_G >> 8) & 0xFF,
      LED_B & 0xFF, (LED_B >> 8) & 0xFF,
    ])
    await this.send('setRGB',Datagram)
  }

/**** Analog Input ****/

  async getAnalog (Port:number):Promise<number> {
    Port = Math.floor(Port)
    if ((Port < 0) || (Port > 3)) throw new Error(
      'multi-io thing: invalid analog input port ' + Port
    )

    const Data = await this.send('getAnalog',new Uint8Array([Port]))
    return (Data[0] + Data[1]*255) / 4096
  }

/**** Servo Control ****/

  async setServo (Port:number, Angle:number):Promise<void> {
    Port = Math.floor(Port)
    if ((Port < 0) || (Port > 7)) throw new Error(
      'multi-servo thing: invalid servo port ' + Port
    )

    Angle = Math.floor(Math.max(0,Math.min(Angle,255)))
    await this.send('setServo',new Uint8Array([Port,Angle]))
  }

/**** API Specification ****/

  public api = [{
    name: 'setRGB',
    args: [
      'R: 0 to 1',
      'G: 0 to 1',
      'B: 0 to 1'
    ]
  },{
    name:  'getAnalog',
    args:  [ 'port: 0 to 3' ],
    return:'0 to 1'
  },{
    name: 'setServo',
    args: [ 'port: 0 to 7', 'angle: 0 to 180 (technically up to 255)' ]
  }]
}
