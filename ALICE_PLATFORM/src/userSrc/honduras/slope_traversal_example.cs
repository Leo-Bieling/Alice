List <Point3d> curpts;
List <Polyline> polys;

private void RunScript(Surface srf, List<Point3d> pts, double rot, int numsteps, double diststep, ref object A)
{
    //init
    polys = new List <Polyline>();

    //load up
    curpts = pts;

    for (int i = 0; i < pts.Count; i++)
    {
      Polyline p = new Polyline();
      polys.Add(p);
    }

    for (int i = 0; i < polys.Count; i++)
    {
      Polyline p = polys[i];
      p.Add(pts[i]);
    }

    //execute
    //closest point
    double u;
    double v;
    Point3d outPt;
    Vector3d [] derivatives;
    Vector3d normal;
    Point3d newPos;

    for(int j = 0; j < numsteps; j++)
    {
      for(int i = 0; i < polys.Count; i++)
      {
        srf.ClosestPoint(curpts[i], out u, out v);
        normal = srf.NormalAt(u, v);
        srf.Evaluate(u, v, 1, out outPt, out derivatives);
        Vector3d vec;
        vec = Vector3d.CrossProduct(normal, new Vector3d(0, 0, 1));
        vec.Unitize();
        vec.Rotate(rot, normal);
        vec = vec * diststep;
        newPos = curpts[i] + vec;

        //drop in the poly
        Polyline p = polys[i];
        p.Add(newPos);

        //reassign val for next loop
        curpts[i] = newPos;
      }
    }
    
    A = polys;
}
